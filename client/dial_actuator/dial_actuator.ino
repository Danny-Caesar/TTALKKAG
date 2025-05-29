#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <esp_sleep.h>
#include <Stepper.h>
#include <sstream>
#include <vector>

// 디바이스 정보
const char* client_id = "da0X76";
const char* client_type = "dial_actuator";
char json_payload_connect[256];

// WiFi 정보
const char* ssid = "kdg";
const char* password = "12345678";

// MQTT 정보
const char* mqtt_server = "192.168.137.39"; // MQTT Broker IPv4
const int mqtt_port = 1883;
const char* mqtt_user = NULL;
const char* mqtt_password = NULL;
const char* topic_connect = "hub/connect";
String topic_triggers;
String topic_subscribe;
String topic_unsubscribe;
String topic_disconnect;
String topic_action;
String topic_down;
String topic_up;
String topic_step;

const int REVOLUTION = 2048;
const int SM1 = 1;
const int SM2 = 0;
const int SM3 = 3;
const int SM4 = 2;
const int DOWN = 0;
const int UP = 1;
const int STEP_BASE = 3;
static int step_unit = 1;
static int step = 0;
// IN4, IN2, IN3, IN1
Stepper myStepper(REVOLUTION, SM4, SM2, SM3, SM1);

// MQTT 객체
WiFiClient espClient;
PubSubClient client(espClient);

String get_cmd(const String& data, char delimiter) {
  int start = 0;
  int end = data.indexOf(delimiter);

  start = end + 1;
  end = data.indexOf(delimiter, start);

  String cmd = data.substring(start, end);

  return cmd;
}

// ssid와 password 정보로 WiFi 연결
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // 연결 성공
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // 본 디바이스의 IPv4
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // 서버에 MQTT 연결
    if (client.connect(client_id, mqtt_user, mqtt_password)) {
      // 성공
      Serial.println("connected");
      
      // 연결 토픽 발행 (retain = false, QoS: 1)
      client.publish(topic_connect, (const uint8_t*)json_payload_connect, strlen(json_payload_connect), false);

      // 필요한 토픽 구독
      client.subscribe(topic_triggers.c_str());
      delay(100);
      client.subscribe(topic_subscribe.c_str());
      delay(100);
      client.subscribe(topic_unsubscribe.c_str());
      delay(100);
      client.subscribe(topic_action.c_str());
      delay(100);
      client.subscribe(topic_down.c_str());
      delay(100);
      client.subscribe(topic_up.c_str());
      delay(100);
      client.subscribe(topic_step.c_str());
      delay(100);
      client.subscribe(topic_disconnect.c_str());
    } else {
      // 실패
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in seconds...");
      delay(5000);
    }
  }
}

// 메시지 수신 시 호출되는 함수
void callback(char* topic, byte* message, unsigned int length) {
  // 수신한 토픽 확인
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  
  // 수신한 메시지 확인
  Serial.print(". Message: ");
  String payload;
  for (unsigned int i = 0; i < length; i++) {
    payload += (char)message[i];
  }
  Serial.println(payload);

  String cmd = get_cmd(topic, '/');
  if(strcmp(topic, topic_triggers.c_str()) == 0){
    subscribe_trigger_list(payload);
  }
  else if(strcmp(topic, topic_subscribe.c_str()) == 0){
    subscribe_trigger(payload);
  }
  else if(strcmp(topic, topic_unsubscribe.c_str()) == 0){
    unsubscribe_trigger(payload);
  }
  else if(strcmp(topic, topic_step.c_str()) == 0) {
    set_step(payload);
  }
  else if(strcmp(cmd.c_str(), "action") == 0) {
    // click();
  }
  else if(strcmp(cmd.c_str(), "down") == 0) {
    rotate(DOWN);
  }
  else if(strcmp(cmd.c_str(), "up") == 0) {
    rotate(UP);
  }
  else if(strcmp(topic, topic_disconnect.c_str()) == 0) {
    // disconnect 토픽 처리
    client.disconnect();
    // sleep 상태에 돌입
    esp_deep_sleep_start();
  }
}

// 연결 메시지 초기화
void setup_payload_connect() {
  // 연결 메시지 생성
  StaticJsonDocument<200> doc;
  doc["clientId"] = client_id;
  doc["type"] = client_type;
  
  // Json 컨테이너를 문자열로 변환
  serializeJson(doc, json_payload_connect);
}

// 토픽 초기화
void setup_topic() {
  topic_triggers = String("server/triggers/") + client_type  + "/" + client_id;

  topic_subscribe = String("server/subscribe/") + client_type  + "/" + client_id;
  topic_unsubscribe = String("server/unsubscribe/") + client_type  + "/" + client_id;

  topic_disconnect = String("server/disconnect/") + client_type + "/" + client_id;

  topic_action = String("server/action/") + client_type + "/" + client_id;
  topic_down = String("server/down/") + client_type + "/" + client_id;
  topic_up = String("server/up/") + client_type + "/" + client_id;
  topic_step = String("server/step/") + client_type + "/" + client_id;
}

void subscribe_trigger_list(String triggers){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, triggers);
  JsonArray client_type = doc["clientType"].as<JsonArray>();
  JsonArray client_id = doc["clientId"].as<JsonArray>();

  for(int i=0;i<client_type.size();i++){
    const char* ctype = client_type[i];
    const char* cid = client_id[i];
    String action = String("server/action/") + ctype + "/" + cid;
    String down = String("server/down/") + ctype + "/" + cid;
    String up = String("server/up/") + ctype + "/" + cid;

    Serial.println("subscribed:\n" + action + '\n' + down + '\n' + up + '\n');
    client.subscribe(action.c_str());
    delay(100);
    client.subscribe(down.c_str());
    delay(100);
    client.subscribe(up.c_str());
  }
}

void subscribe_trigger(String trigger){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, trigger);

  const char* ctype = doc["type"];
  const char* cid = doc["client_id"];

  String action = String("server/action/") + ctype + "/" + cid;
  String down = String("server/down/") + ctype + "/" + cid;
  String up = String("server/up/") + ctype + "/" + cid;

  client.subscribe(action.c_str());
  delay(100);
  client.subscribe(down.c_str());
  delay(100);
  client.subscribe(up.c_str());
}

void unsubscribe_trigger(String trigger){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, trigger);

  const char* ctype = doc["type"];
  const char* cid = doc["client_id"];

  String action = String("server/action/") + ctype + "/" + cid;
  String down = String("server/down/") + ctype + "/" + cid;
  String up = String("server/up/") + ctype + "/" + cid;

  client.unsubscribe(action.c_str());
  delay(100);
  client.unsubscribe(down.c_str());
  delay(100);
  client.unsubscribe(up.c_str());
}

void set_step(String payload){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  const int unit = doc["step_unit"];

  if(doc.containsKey("current_step")){
    step = doc["current_step"];
    step_unit = unit;
  }
  else
  {
    if(unit > 100)return;
    else if(unit < 1)return;
    step_unit = unit;

    // Init cur step 0
    int steps = map(-1 * step, -360, 360, -REVOLUTION, REVOLUTION);
    myStepper.step(steps);
  }
}

void rotate(int direction){
  int angle;

  if(direction == DOWN){
    step -= step_unit;
    if(step < step_unit){
      angle = abs(step - step_unit) * STEP_BASE;
    }
    else{
      angle = -1 * STEP_BASE * step_unit;;
    }
  }
  else if(direction == UP){
    step += step_unit;
    angle = STEP_BASE * step_unit;
  }


  int steps = map(angle, -360, 360, -REVOLUTION, REVOLUTION);

  myStepper.step(steps);

  Serial.print(angle);
  Serial.println(" degree rotated.");
  delay(100);
}

void setup() {
  Serial.begin(115200);

  setup_payload_connect();
  setup_topic();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);

  myStepper.setSpeed(10);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <esp_sleep.h>
#include "ESP32Servo.h"

#define SERVO_PIN 4

// 디바이스 정보
const char* client_id = "bc0X00";
const char* client_type = "button_clicker";
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

// Servo 정보
const int angle_initial = 100;
const int angle_target = 75;
const int delay_click = 1000;

// MQTT 객체
WiFiClient espClient;
PubSubClient client(espClient);

// 서보 모터 객체
Servo servo;

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
      delay(500);
      client.subscribe(topic_subscribe.c_str());
      delay(500);
      client.subscribe(topic_unsubscribe.c_str());
      delay(500);
      client.subscribe(topic_action.c_str());
      delay(500);
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
  else if(strcmp(cmd.c_str(), "action") == 0) {
    // action 토픽 처리
    click();
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
  topic_subscribe = String("server/unsubscribe/") + client_type  + "/" + client_id;

  // disconnect 토픽 설정
  topic_disconnect = String("server/disconnect/") + client_type + "/" + client_id;

  // action 토픽 설정
  topic_action = String("server/action/") + client_type + "/" + client_id;
}

void subscribe_trigger_list(String triggers){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, triggers);
  JsonArray client_type = doc["clientType"].as<JsonArray>();
  JsonArray client_id = doc["clientId"].as<JsonArray>();

  for(int i=0;i<client_type.size();i++){
    const char* ctype = client_type[i];
    const char* cid = client_id[i];
    String action = String("client/action/") + ctype + "/" + cid;

    Serial.println("subscribed: " + trigger);
    client.subscribe(action.c_str());
  }
}

void subscribe_trigger(String trigger){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, trigger);

  const char* ctype = doc["type"];
  const char* cid = doc["client_id"];

  String action = String("server/action/") + ctype + "/" + cid;

  Serial.println("subscribed: " + action);
  client.subscribe(action.c_str());
}

void unsubscribe_trigger(String trigger){
  StaticJsonDocument<200> doc;
  deserializeJson(doc, trigger);

  const char* ctype = doc["type"];
  const char* cid = doc["client_id"];

  String action = String("server/action/") + ctype + "/" + cid;

  client.unsubscribe(action.c_str());
}

// 서보 모터 초기화
void setup_servo() {
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(angle_initial);
}

// 클릭 동작
void click() {
  servo.write(angle_target);
  delay(delay_click);
  servo.write(angle_initial);
  delay(delay_click);
}

void setup() {
  Serial.begin(115200);

  setup_payload_connect();
  setup_topic();

  setup_wifi();

  setup_servo();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

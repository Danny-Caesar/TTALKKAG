#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <esp_sleep.h>

#define INPUT_DELAY 50

// 디바이스 정보
const char* client_id = "rc0X99";
const char* client_type = "remote";
char json_payload_connect[256];

// WiFi 정보
const char* ssid = "kdg";
const char* password = "12345678";

// MQTT 정보
const char* mqtt_server = "192.168.137.67"; // MQTT Broker IPv4
const int mqtt_port = 1883;
const char* mqtt_user = NULL;
const char* mqtt_password = NULL;
const char* topic_connect = "hub/connect";
String topic_disconnect;
String topic_remote[3];

// MQTT 객체
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long millis_last = 0;

const int button_pin[3] = {10, 20, 21};
const int action_pin= 0;
const int down_pin = 1;
const int up_pin = 2;

int state[3] = {1, 1, 1};
int last_state[3] = {1, 1, 1};

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
      
      // 연결 토픽 발행 (retain = false, QoS: 0)
      client.publish(topic_connect, (const uint8_t*)json_payload_connect, strlen(json_payload_connect), false);

      // 필요한 토픽 구독
      client.subscribe(topic_disconnect.c_str(), 1);
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

  if(strcmp(topic, topic_disconnect.c_str()) == 0) {
    // disconnect 토픽 처리
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
  // disconnect 토픽 설정
  topic_disconnect = String("server/disconnect/") + client_type + "/" + client_id;

  // remote 토픽 설정
  String topic_action = String("server/action/") + client_type + "/" + client_id;
  String topic_down = String("server/down/") + client_type + "/" + client_id;
  String topic_up = String("server/up/") + client_type + "/" + client_id;

  topic_remote[action_pin] = topic_action;
  topic_remote[down_pin] = topic_down;
  topic_remote[up_pin] = topic_up;
}

bool debounce(int pin, bool state_last){
  bool state_current = digitalRead(pin);
  if(state_last != state_current){
    delay(5);
    state_current = digitalRead(pin);
  }
  return state_current;
}

void setup() {
  for(int i=0;i<3;i++){
    pinMode(button_pin[i], INPUT_PULLUP);
  }

  setup_payload_connect();
  setup_topic();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);

  Serial.begin(115200);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long millis_current = millis();

  if(millis_current - millis_last >= INPUT_DELAY){
    for(int i=0;i<3;i++){
      state[i] = debounce(button_pin[i], last_state[i]);

      if(last_state[i] == HIGH && state[i] == LOW){
        Serial.print("button");
        Serial.print(i);
        Serial.println(" clicked.");
        client.publish(topic_remote[i].c_str(), (const uint8_t*)"", 0, false);
        last_state[i] = state[i];
        break;
      }
      last_state[i] = state[i];
    }

    // for(int i=0;i<3;i++){
    //   Serial.print(state[i]);
    //   Serial.print(" ");
    // }
    // Serial.println();

    millis_last = millis_current;
  }
}
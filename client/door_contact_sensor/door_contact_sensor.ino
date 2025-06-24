#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <esp_sleep.h>

#define REED_PIN 4
#define INPUT_DELAY 500
#define DETECT_DELAY 3000

// 디바이스 정보
const char* client_id = "dcs0X00";
const char* client_type = "door_sensor";
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
String topic_disconnect;
String topic_open;

// Reed 정보
bool door_open = false;
bool reed_current = LOW;
bool reed_last = LOW;

// MQTT 객체
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long millis_last = 0;

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

// 토픽 초기화
void setup_topic() {
  // disconnect 토픽 설정
  topic_disconnect = String("server/disconnect/") + device_type + "/" + client_id;

  // open 토픽 설정
  topic_open = String("client/action/") + device_type + "/" + client_id;
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
  Serial.begin(115200);

  setup_topic();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(REED_PIN, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long millis_current = millis();

  if(millis_current - millis_last >= DELAY){
    reed_current = debounce(REED_PIN, reed_last);
    if(reed_last == LOW && reed_current == HIGH){
      client.publish(topic_open.c_str(), (const uint8_t*)"", 0, false);
      delay(DETECT_DELAY);
    }

    reed_last = reed_current;
    millis_last = millis_current;
  }
}

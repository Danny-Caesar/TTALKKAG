#include <WiFi.h>
#include <PubSubClient.h>
#include "ESP32Servo.h"

#define SERVO_PIN 5

// WiFi 정보
const char* ssid = "kdg";
const char* password = "12345678";

// MQTT 정보
const char* mqtt_server = "192.168.137.39"; // MQTT Broker IPv4
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";
const char* topic_click = "server/click";

// Servo 정보
const int angle_initial = 0;
const int angle_target = 45;
const int delay_click = 1000;

// MQTT 객체
WiFiClient espClient;
PubSubClient client(espClient);

// 서보 모터 객체
Servo servo;

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

  // Click 토픽 처리
  if (strcmp(topic, topic_click) == 0) {
    click();
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Client id 할당
    String clientId = "button_clicker_";
    clientId += String(random(0xffff), HEX);

    // 서버에 MQTT 연결
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      // 성공
      Serial.println("connected");
      
      // 필요한 토픽 구독
      client.subscribe(topic_click, 1);
    } else {
      // 실패
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in a second...");
      delay(1000);
    }
  }
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
  setup_servo();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

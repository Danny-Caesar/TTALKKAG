#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// 디바이스 정보
const char* client_id = "test0X00";
const char* client_type = "button_clicker";
char json_payload_connect[256];

// WiFi 정보
const char* ssid = "kdg";
const char* password = "12345678";

// MQTT 정보
const char* MQTT_SERVER = "192.168.137.67"; // MQTT Broker IPv4
const int MQTT_PORT = 1883;
const char* MQTT_USER = NULL;
const char* MQTT_PASSWORD = NULL;
const int DELAY_SUB = 500;
const char* topic_connect = "hub/connect";
String topic_disconnect;

// MQTT 객체
WiFiClient espClient;
PubSubClient client;

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

    if (client.connect(client_id, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");

      client.publish(topic_connect, (const uint8_t*)json_payload_connect, strlen(json_payload_connect), false);

      client.subscribe(topic_disconnect.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  
  Serial.print(". Message: ");
  String payload;
  for (unsigned int i = 0; i < length; i++) {
    payload += (char)message[i];
  }
  Serial.println(payload);

  if(strcmp(topic, topic_disconnect.c_str()) == 0) {
    client.unsubscribe(topic_disconnect.c_str());
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

void setup_topic() {
  topic_disconnect = String("server/disconnect/") + client_type + "/" + client_id;
}

void setup() {
  Serial.begin(115200);

  setup_payload_connect();
  setup_topic();

  setup_wifi();

  client.setClient(espClient);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  client.setKeepAlive(60);

  Serial.println(client.getBufferSize());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

const int button_pin[3] = {10, 20, 21};
const int power_pin= 0;
const int down_pin = 1;
const int up_pin = 2;

int state[3] = {0, 0, 0};

void setup() {
  for(int i=0;i<3;i++){
    pinMode(button_pin[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
}

void loop() {
  for(int i=0;i<3;i++){
    state[i] = digitalRead(button_pin[i]);
    Serial.print(state[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(100); // 너무 빠른 반복 방지
}
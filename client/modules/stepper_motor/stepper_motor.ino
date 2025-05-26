#include <Stepper.h>

// 스텝모터의 전체 스텝 수 (28BYJ-48: 2048 스텝 = 360도)
const int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution, 1, 0, 3, 2);

void setup() {
  Serial.begin(115200);
  Serial.println("원하는 각도를 입력하세요 (예: 90 또는 -180):");
  myStepper.setSpeed(10); // 속도 설정 (RPM)
}

void loop() {
  if (Serial.available() > 0) {
    int angle = Serial.parseInt(); // 시리얼 입력된 각도 읽기
    int steps = map(angle, -360, 360, -stepsPerRevolution, stepsPerRevolution); // 각도를 스텝 수로 변환

    myStepper.step(steps);

    Serial.print("모터가 ");
    Serial.print(angle);
    Serial.println("도 회전했습니다.");
    delay(500); // 다음 입력을 위한 대기
  }
}

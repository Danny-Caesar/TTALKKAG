#include <Stepper.h>

#define DOWN 0
#define UP 1

// 스텝모터의 전체 스텝 수 (28BYJ-48: 2048 스텝 = 360도)
const int stepsPerRevolution = 2048;
// IN4, IN2, IN3, IN1
Stepper myStepper(stepsPerRevolution, 2, 0, 3, 1);

const int step_base = 3;
static int step_unit = 1;

void set_step_unit(int val){
  if(val > 100)return;
  else if(val < 1)return;
  step_unit = val;
}

void rotate(int direction){
  int angle = step_base * step_unit;
  if(direction == DOWN){
    angle *= -1;
  }

  int steps = map(angle, -360, 360, -stepsPerRevolution, stepsPerRevolution);

  myStepper.step(steps);

  Serial.print(angle);
  Serial.println(" degree rotated.");
  delay(100);
}

void setup() {
  Serial.begin(115200);

  myStepper.setSpeed(10); // RPM
  Serial.println("step_unit: ");
}

void loop() {
  if (Serial.available() > 0) {
    int unit = Serial.parseInt();
    set_step_unit(unit);
    rotate(UP);
    rotate(DOWN);
  }
}

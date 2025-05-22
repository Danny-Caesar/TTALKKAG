#include <Stepper.h>
#define STEPS 2037 // , 한바퀴를 이루는 스텝 갯수 입력
Stepper stepper(STEPS, 8, 10, 9, 11); //고정자 권선 순서 설정
void setup(){
  stepper.setSpeed(12); // 회전 속도 지정
}
void loop() {
  stepper.step(STEPS); // 정방향 회전
  delay(1000);
  stepper.step(-STEPS); // 역방향 회전
  delay(1000);
}
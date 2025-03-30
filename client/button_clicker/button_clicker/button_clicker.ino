/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  
int initialAngle = 0;  // 서보모터의 초기 각도
int returnAngle = 45;  // 서보모터가 돌아올 각도 (목표 각도)

void setup() {
  // 서보모터를 3번 핀에 연결
  myservo.attach(3);
  
  // 서보모터를 초기 각도로 설정
  myservo.write(initialAngle);
  
  // 서보모터 초기화 후 1초 대기
  delay(1000);  // 1초 대기
}

void loop() {
  // 서보모터를 목표 각도로 이동 (예: 90도)
  myservo.write(returnAngle);
  delay(1000);  // 1초 대기

  // 서보모터를 초기 각도로 돌아가게 이동 (예: 0도)
  myservo.write(initialAngle);
  delay(1000);  // 1초 대기
}

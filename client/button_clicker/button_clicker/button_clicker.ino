/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  


void setup() {
  myservo.attach(3);  
}

void loop() {   
  myservo.write(150);                  
  delay(15);         
  myservo.write(30);                  
  delay(15);                  
}

#include <Servo.h>

int SERVOPIN = 11;
Servo shutter;



void setup(){
  
shutter.attach(SERVOPIN);
Serial.begin(57600);


}


////////////////////////////////////////
void loop(){
  
  
  while(Serial.available()>0){
  int value = Serial.read();
  shutter.write(value);
  }

}

// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object 
   Serial.begin(9600);
  establishContact();  // send a byte to establish contact until receiver

} 
 
 
void loop() 
{ 
     if (Serial.available() > 0)
     {
    
      int newpos = Serial.read();
      int distance = abs(newpos - pos);
      int sens = 0;
      if( newpos - pos > 0 ) { sens = 1 ;}
      pos = newpos;
      
      if( distance > 10)
      {
            for(int x = 0; x < distance; x += 1)  // goes from 0 degrees to 180 degrees 
            {                                  // in steps of 1 degree 
            
            if(sens > 0 ){
            myservo.write(pos- distance + x);
            }
            else{
            myservo.write(pos + distance - x);
            }

            delay(10);
            }
        
           
            

     }
     else{
      myservo.write(pos);
     }
     
     } 
     


} 

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

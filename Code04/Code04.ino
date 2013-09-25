#include <Servo.h>

Servo myServo;    //create a new servo object
int pos = 0;      //store servo position


void setup() {
  Serial.begin(9600);
  myServo.attach(9); //attaches the servo on pin 9 to the servo object 
}  

void loop() {
 myServo.write(0);
 delay(2000);
 myServo.write(+20);
 delay(5000);
 myServo.write(0);
 delay(5000);
 
}
void turn180andBack() {
 Serial.println("Executing 0-180...");
  for (pos = 0; pos <= 180; pos += 1){
      myServo.write(pos);
      Serial.println(pos);
      delay(25);
  }
  Serial.println("Executing 180-0...");
  for (pos = 180; pos >= 1; pos -=1){
     myServo.write(pos);
     Serial.println(pos);
    delay(25); 
  } 
  
}

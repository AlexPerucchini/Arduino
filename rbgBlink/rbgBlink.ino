/* RGB Blink */

int del = 1000;
void setup () {
  
  pinMode(9, OUTPUT);   
  pinMode(10, OUTPUT);   
  pinMode(11, OUTPUT);   
}
void loop() {
  digitalWrite(9, HIGH);   // turns on red
  digitalWrite(11, LOW);   // turns off blue
  delay(del);           
  digitalWrite(10, HIGH);  // turns on green
  digitalWrite(9, LOW);    // turns off red
  delay(del);           
  digitalWrite(11, HIGH);  // turns on blue
  digitalWrite(10, LOW);   // turns off green
  delay(del);              
}



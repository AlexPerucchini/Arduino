//Potentiometer Reading Sketch

const int POT = 0;
const int LED = 11;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(POT);
  int lightLevel = map(potValue, 0, 1023, 0, 255);
  lightLevel = constrain(lightLevel, 0,255);

  analogWrite(LED,lightLevel);
  Serial.println(potValue);
}

//Exploring arduino
//Project 3
//Analog Sensor


int sensePin  = 0;
int ledPin    = 11;

void setup() {
  /* code */
  analogReference(DEFAULT);
  Serial.begin(9600);
  pinMode (ledPin, OUTPUT);
}

void loop() {
  /* code */
  int lightLevel = analogRead(sensePin);
  //adjust light level 0 to 900 to 0 255
  lightLevel = map(lightLevel, 0, 900, 0, 255);
  lightLevel = constrain(lightLevel, 0,255);
  String sensePinStr = "Sense Pin: ";
  String lightLevelStr = "Light level: ";
  int sensePinValue =  analogRead(sensePin);
  Serial.println( sensePinStr + sensePinValue);
  Serial.println ( lightLevelStr + lightLevel);
  //Serial.println("lightLevel" + lightLevel);
  delay(1000);
  analogWrite(ledPin, lightLevel);
}

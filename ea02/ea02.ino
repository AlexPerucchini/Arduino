//Exploring arduino
//Project 2
//On-off button to blink LED

int switchPin = 8;
int ledPin = 11;
int ledPin2 = 13;
boolean lastButton = LOW;
//boolean ledOn = false;
int ledLevel  = 0;
boolean currentButton = LOW;

void setup() {
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

//this function will prevent switch bouncing in software
boolean debounce(boolean last) {

  boolean current = digitalRead(switchPin);
  if (last != current) {
    //had to increase the delay +5 in order to prevent switch bounce
    //the original delay was 5
    delay(10);
    current = digitalRead(switchPin);
  }
  return current;

}

void loop() {
  digitalWrite(ledPin2, HIGH);
  currentButton  = debounce(lastButton);
  if(lastButton == LOW && currentButton == HIGH){
    ledLevel = ledLevel + 21;
  }
  lastButton =  currentButton;
  if (ledLevel > 255) ledLevel = 0;
  analogWrite(ledPin, ledLevel);
}







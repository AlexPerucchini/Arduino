/*
 Code02
*/

// LED connected to digital pin array
int ledPins[] = {  3, 5, 6, 9, 10};

void setup()  { 
  //initialize ledPins Array
  for (int i = 0; i < 5; ++i)
  {
    pinMode(ledPins[i], OUTPUT);
  }
} 

void loop()  { 
  // make the magic happen
 // oneAfterAnother();
 // oneAfterAnotherReverse();
   //delay(1000);
 fadeLoop(); 
  //delay(1000);
 //oneAfterAnother();
  //oneAfterAnother();
}
//===============================================================================
void oneAfterAnotherReverse() {

  int delayTime = 100;
 
  for (int i = 6; i >= 0; --i)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(delayTime);
    digitalWrite(ledPins[i], LOW);
  }
  
}

//===============================================================================
void oneAfterAnother() {

  int delayTime = 50;
 
  for (int i = 0; i <=  5; ++i)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(delayTime);
    digitalWrite(ledPins[i], LOW);
  }
}
//===============================================================================
void fadeLoop() {

  int delayTime = 30;
 
   for (int i = 0; i < 5; ++i)
  {
    //fade in
    for (int fade = 0; fade <= 255; fade +=5)
    {
      analogWrite(ledPins[i], fade);
      delay(delayTime);
    }
    //fade out
    for (int fade = 255; fade >= 0; fade -=5)
    {
      analogWrite(ledPins[i], fade);
      delay(delayTime);
    }
  }
   //delay(delayTime);
}



/*
wallyFunctions.pde
Author: Alex Perucchini
This .pde file gets concatenated at the end of the Wally.ino file to separate
all functions calls from the main driver
*/

//=============================================================================
// Explore
//=============================================================================
void explore()
{
  int minSafeDist = 15;

  lookAhead();
  if (frontDistance >= minSafeDist)
  {
    goForward();
    delay(110);
  }
  else
  {
    brake();
    lookAround();
    if (rightDistance > leftDistance && rightDistance > minSafeDist)
    {
      turnRight();
      //force the program to return to the top of the loop
      return;
    }
    if (leftDistance > rightDistance && leftDistance > minSafeDist)
    {
      turnLeft();
      return;
    }
    if (frontDistance <= 3)
    {
      goBackwards();
    }
    reverse();
  }
}
//=============================================================================
// PING
//=============================================================================
long ping()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  return inches;
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void turnServoRight()
{
  //scan right
  myServo.write(120);
  delay(600);
}

void turnServoLeft()
{
  //scan left
  myServo.write(30);
  delay(600);
}

void turnServoCenter()
{
  //center the servo
  myServo.write(83);
  delay(175);
}
//=============================================================================
// Motor functions
//=============================================================================
void goForward()
{
  //Get the distance from the sensor and map it to achieve
  //a gradual increase to the motor speed
  //Drive both motors CW, full speed 255, half speed 127s
  motorDrive(motor1, turnCW, 200);
  motorDrive(motor2, turnCW, 200);
  Serial.println("goForward");

}
void lookAhead()
{
  turnServoCenter();
  frontDistance = ping();
  Serial.print("lookAhead -> frontDistance (inches): ");
  Serial.println(frontDistance);
}

void lookAround()
{
  turnServoRight();
  rightDistance = ping();
  turnServoLeft();
  leftDistance = ping();
  turnServoCenter();
  frontDistance = ping();
  Serial.print("lookAround -> frontDistance (inches): ");
  Serial.println(frontDistance);
  Serial.print("lookAround -> leftDistance(inches): ");
  Serial.println(leftDistance);
  Serial.print("lookAround -> rightDistance (inches): ");
  Serial.println(rightDistance);
}

void goBackwards()
{
  //Drive both motors CCW, full speed 255, half speed 127
  motorDrive(motor1, turnCCW, 150);
  motorDrive(motor2, turnCCW, 150);
  beep();
  blink();
  Serial.println("goBackwards");
  delay(400);
}

void reverse()
{
  //Do a tight turn towards motor1: Motor2 forward, Motor1 reverse
  motorDrive(motor1, turnCCW, 160);
  motorDrive(motor2, turnCW, 160);
  beep();
  Serial.println("goBackwards");
  delay(400);
}

void turnLeft()
{
  //Turn towards motor1: Stop Motor1, slow Motor2
  motorStop(motor1);
  motorDrive(motor2, turnCW, 160);
  Serial.println("turnLeft");
  delay(400);
}

void turnRight()
{
  //Turn towards motor2: Stop Motor2, slow Motor2
  motorStop(motor2);
  motorDrive(motor1, turnCW, 160);
  Serial.println("turnRight");
  delay(400);
}

void brake()
{
  motorBrake(motor1);
  motorBrake(motor2);
  motorsStandby();
}

void motorDrive(boolean motorNumber, boolean motorDirection, int motorSpeed)
{
  /*
  This Drives a specified motor, in a specific direction, at a specified speed:
    - motorNumber: motor1 or motor2 ---> Motor 1 or Motor 2
    - motorDirection: turnCW or turnCCW ---> clockwise or counter-clockwise
    - motorSpeed: 0 to 255 ---> 0 = stop / 255 = fast
  */

  boolean pinIn1;  //Relates to AIN1 or BIN1 (depending on the motor number specified)

  //Specify the Direction to turn the motor
  //Clockwise: AIN1/BIN1 = HIGH and AIN2/BIN2 = LOW
  //Counter-Clockwise: AIN1/BIN1 = LOW and AIN2/BIN2 = HIGH
  if (motorDirection == turnCW)
    pinIn1 = HIGH;
  else
    pinIn1 = LOW;

  //Select the motor to turn, and set the direction and the speed
  if(motorNumber == motor1)
  {
    digitalWrite(pinAIN1, pinIn1);
    digitalWrite(pinAIN2, !pinIn1);  //This is the opposite of the AIN1
    analogWrite(pinPWMA, motorSpeed);
  }
  else
  {
    digitalWrite(pinBIN1, pinIn1);
    digitalWrite(pinBIN2, !pinIn1);  //This is the opposite of the BIN1
    analogWrite(pinPWMB, motorSpeed);
  }

  //Finally , make sure STBY is disabled - pull it HIGH
  digitalWrite(pinSTBY, HIGH);
}

void motorBrake(boolean motorNumber)
{
  //This "Short Brake"s the specified motor, by setting speed to zero

  if (motorNumber == motor1)
    analogWrite(pinPWMA, 0);
  else
    analogWrite(pinPWMB, 0);
}

void motorStop(boolean motorNumber)
{
  // This stops the specified motor by setting both IN pins to LOW
  if (motorNumber == motor1) {
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, LOW);
  }
  else
  {
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, LOW);
  }
}

void motorsStandby()
{
  //This puts the motors into Standby Mode
  digitalWrite(pinSTBY, LOW);
}
//=============================================================================
// Status LED blink
//=============================================================================
void blink()
{
  for(int i = 0; i < 5; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(60);
    digitalWrite(ledPin, LOW);
    delay(60);
  }
}
//=============================================================================
// Piezo beep
//=============================================================================
void beep(){
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(piezoPin, HIGH);
    delay(50);
    digitalWrite(piezoPin, LOW);
    delay(50);
  }
  blink();
}
//==============================================================================
// Telemetry
//==============================================================================
void telemetry()
{
  getAmbientTemp();
}

void getAmbientTemp()
{
  //int v = 0;
  int v = analogRead(tempPin);

  Serial.print("temp sensor: ");
  Serial.println(v);

  //get the voltage
  float volts = calculate_voltage(v);
  Serial.print("voltage: ");
  Serial.println(volts);

  //get temperature celsius
  float cel = temp_cel(volts);
  Serial.print("temp in celsius: ");
  Serial.println(cel);


  // now convert to Fahrenheit
  float f = (cel * 9.0 / 5.0) + 32.0;
  Serial.print("temp in fahrenheit: ");
  Serial.println(f);

}
float calculate_voltage(int value)
{
  float voltage  = value * 5.0;
  voltage = voltage / 1024.0;
  return voltage;
}

float temp_cel(float volts)
{
  float tempC = (volts - 0.5) * 100;
  return tempC;
}

//==============================================================================
// Information
//==============================================================================
void information()
{
  int i,j;
  int data1,data2,data3,data4;
  unsigned long ul;
  double percentage;

  Serial.println(F(""));
  Serial.println(F("Information"));
  Serial.println(F("-----------"));

  Serial.print(F("sketch Size = "));
  ul = sketchSize();
  Serial.print(ul,DEC);
  Serial.print(F(" ("));
  percentage = (double) ul / ((double) FLASHEND + 1.0) * 100.0;
  Serial.print(percentage,0);
  Serial.println(F("%)"));

  Serial.print(F("free RAM    = "));
  i = freeRam();
  Serial.println(i,DEC);
  Serial.print(F("RAM used    = "));
  j = (RAMEND + 1) - i;
  Serial.print(j, DEC);
  Serial.print(F(" ("));
  percentage = (double) j / ((double) RAMEND + 1.0) * 100.0;
  Serial.print(percentage,0);
  Serial.println(F("%)"));

  Serial.print(F("__VERSION__ = "));
  Serial.println(F(__VERSION__));

  Serial.print(F("__DATE__    = "));
  Serial.println(F(__DATE__));

  Serial.print(F("__TIME__    = "));
  Serial.println(F(__TIME__));

  Serial.print(F("__AVR_LIBC_VERSION_STRING__ = "));
  Serial.println(F(__AVR_LIBC_VERSION_STRING__));

  Serial.print(F("__FILE__    = "));
  Serial.println(F(__FILE__));

  Serial.print(F("__STDC__    = "));
  Serial.println(__STDC__,DEC);

  Serial.print(F("OSCCAL = "));
  Serial.println(OSCCAL,DEC);

  Serial.print(F("GPIOR0 = 0x"));
  Serial.println(GPIOR0,HEX);

  Serial.print(F("GPIOR1 = 0x"));
  Serial.println(GPIOR1,HEX);

  Serial.print(F("GPIOR1 = 0x"));
  Serial.println(GPIOR1,HEX);

  Serial.print(F("RAMEND   = 0x"));
  Serial.println(RAMEND,HEX);

  Serial.print(F("XRAMEND  = 0x"));
  Serial.println(XRAMEND,HEX);

  Serial.print(F("E2END    = 0x"));
  Serial.println(E2END,HEX);

  Serial.print(F("FLASHEND = 0x"));
  Serial.println(FLASHEND,HEX);

  cli();
  data1 = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
  data2 = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
  data3 = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
  data4 = boot_lock_fuse_bits_get(GET_LOCK_BITS);
  sei();

  Serial.print(F("LOW FUSE      = 0x"));
  Serial.println(data1,HEX);

  Serial.print(F("HIGH FUSE     = 0x"));
  Serial.println(data2,HEX);

  Serial.print(F("EXTENDED FUSE = 0x"));
  Serial.println(data3,HEX);

  Serial.print(F("LOCK BITS     = 0x"));
  Serial.println(data4,HEX);

  Serial.print(F("Processor according to compiler  = "));
#if defined (__AVR_ATtiny45__)
  Serial.println(F("__AVR_ATtiny45__"));
#elif defined (__AVR_ATtiny85__)
  Serial.println(F("__AVR_ATtiny85__"));
#elif defined (__AVR_ATtiny2313__)
  Serial.println(F("__AVR_ATtiny2313__"));
#elif defined (__AVR_ATtiny2313A__)
  Serial.println(F("__AVR_ATtiny2313A__"));
#elif defined (__AVR_ATmega48__)
  Serial.println(F("__AVR_ATmega48__"));
#elif defined (__AVR_ATmega48A__)
  Serial.println(F("__AVR_ATmega48A__"));
#elif defined (__AVR_ATmega48P__)
  Serial.println(F("__AVR_ATmega48P__"));
#elif defined (__AVR_ATmega8__)
  Serial.println(F("__AVR_ATmega8__"));
#elif defined (__AVR_ATmega8U2__)
  Serial.println(F("__AVR_ATmega8U2__"));
#elif defined (__AVR_ATmega88__)
  Serial.println(F("__AVR_ATmega88__"));
#elif defined (__AVR_ATmega88A__)
  Serial.println(F("__AVR_ATmega88A__"));
#elif defined (__AVR_ATmega88P__)
  Serial.println(F("__AVR_ATmega88P__"));
#elif defined (__AVR_ATmega88PA__)
  Serial.println(F("__AVR_ATmega88PA__"));
#elif defined (__AVR_ATmega16__)
  Serial.println(F("__AVR_ATmega16__"));
#elif defined (__AVR_ATmega168__)
  Serial.println(F("__AVR_ATmega168__"));
#elif defined (__AVR_ATmega168A__)
  Serial.println(F("__AVR_ATmega168A__"));
#elif defined (__AVR_ATmega168P__)
  Serial.println(F("__AVR_ATmega168P__"));
#elif defined (__AVR_ATmega32__)
  Serial.println(F("__AVR_ATmega32__"));
#elif defined (__AVR_ATmega328__)
  Serial.println(F("__AVR_ATmega328__"));
#elif defined (__AVR_ATmega328P__)
  Serial.println(F("__AVR_ATmega328P__"));
#elif defined (__AVR_ATmega32U2__)
  Serial.println(F("__AVR_ATmega32U2__"));
#elif defined (__AVR_ATmega32U4__)
  Serial.println(F("__AVR_ATmega32U4__"));
#elif defined (__AVR_ATmega32U6__)
  Serial.println(F("__AVR_ATmega32U6__"));
#elif defined (__AVR_ATmega128__)
  Serial.println(F("__AVR_ATmega128__"));
#elif defined (__AVR_ATmega1280__)
  Serial.println(F("__AVR_ATmega1280__"));
#elif defined (__AVR_ATmega2560__)
  Serial.println(F("__AVR_ATmega2560__"));
#endif

#ifdef SIGRD
  Serial.print(F("SIGRD = "));
  Serial.println(SIGRD,DEC);
#else
  Serial.println(F("SIGRD : not defined (let's make it 5 and see what happens)."));
#define SIGRD 5
#endif

  Serial.print(F("Signature = 0x"));

  data1 = boot_signature_byte_get(0x00);
  data2 = boot_signature_byte_get(0x02);
  data3 = boot_signature_byte_get(0x04);
  data4 = boot_signature_byte_get(0x01);

  Serial.print(data1,HEX);
  Serial.print(F(", 0x"));
  Serial.print(data2,HEX);
  Serial.print(F(", 0x"));
  Serial.println(data3,HEX);

  Serial.print(F("calibration = "));
  Serial.println(data3,DEC);

  Serial.print(F("Number of seconds since start = "));
  Serial.println(millis()/1000L,DEC);

  //Serial.print(F("Internal Temperature = "));
  //Serial.print(GetTemp(),1);
  //Serial.println(F(" Celsius   (the offset could be wrong)."));
  showMenu();
}

double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // This code is not valid for the Arduino Mega,
  // and the Arduino Mega 2560.

#ifdef THIS_MIGHT_BE_VALID_IN_THE_FUTURE
  analogReference (INTERNAL);
  delay(20);            // wait for voltages to become stable.
  wADC = analogRead(8); // Channel 8 is temperature sensor.
#else
  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
#if defined (__AVR_ATmega32U4__)
  wADC = ADC;      // For Arduino Leonardo
#else
  wADC = ADCW;     // 'ADCW' is preferred over 'ADC'
#endif
#endif

  // The offset of 337.0 could be wrong. It is just an indication.
  t = (wADC - 337.0 ) / 1.22;

  return (t);
}



// Helper function for free ram.
//   With use of http://playground.arduino.cc/Code/AvailableMemory
//
int freeRam(void)
{
  extern unsigned int __heap_start;
  extern void *__brkval;

  int free_memory;
  int stack_here;

  if (__brkval == 0)
    free_memory = (int) &stack_here - (int) &__heap_start;
  else
    free_memory = (int) &stack_here - (int) __brkval;

  return (free_memory);
}

// Helper function for sketch size.
// The sketch size is runtime calculated.
// From user "Coding Badly" in his post:
//   http://arduino.cc/forum/index.php/topic,115870.msg872309.html#msg872309
// Changed into unsigned long for code size larger than 64kB.
//
// This function returns the sketch size
// for a size between 0 and 32k. If the code
// size is larger (for example with an Arduino Mega),
// the return value is not valid.
//
unsigned long sketchSize(void)
{
  extern int _etext;
  extern int _edata;

  return ((unsigned long)(&_etext) + ((unsigned long)(&_edata) - 256L));
}



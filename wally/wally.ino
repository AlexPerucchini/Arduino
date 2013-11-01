/* ============================================================================

Wally.ino
Main driver sketch
Author: Alex Perucchini
09-30-2013

This sketch is the first attempt at an autonomous rover. The sketch reads a
PING))) ultrasonic rangefinder and returns the  distance to the closest object
in range. To do this, it sends a pulse to the sensor to initiate a reading,
then listens for a pulse to return.  The length of the returning pulse is
proportional to the distance of the object from the sensor.
The Sparkfun TB6612FNG 1A Dual Motor Driver is used to
initiate basic motor  dmanuvers.

Functions are in two separate .pde files. All program main functions are in the
wallyFunctions.pde file. The system menu is served up by the wallyMenu.pde
file.

Connections:
- Pin 3   ---> PWMA
- Pin 8   ---> AIN2
- Pin 9   ---> AIN1
- Pin 10  ---> STBY
- Pin 11  ---> BIN1
- Pin 12  ---> BIN2
- Pin 5   ---> PWMB
- Pin 7   ---> PING)))
- Pin 12  ---> led

- Motor 1: A01 and A02
- Motor 2: B01 and B02

The Ping))) circuit:
- +V connection of the PING))) attached to +5V
- GND connection of the PING))) attached to ground
- SIG connection of the PING))) attached to digital pin 7

============================================================================*/

#include <avr/boot.h>
#include <Wire.h>
#include <Servo.h>

const int       servoPin = 2;
const int       piezoPin = 4;
const int       pingPin  = 7;
const int       ledPin   = 13;
const int       tempPin  = 0;  //analog Pin

static boolean  turnCW   = 0;  //for motorDrive function
static boolean  turnCCW  = 1;  //for motorDrive function
static boolean  motor1   = 0;  //for motorDrive, motorStop, motorBrake functions
static boolean  motor2   = 1;  //for motorDrive, motorStop, motorBrake functions

//Servo
int pos = 0;
Servo myServo;

//Motor 1
int pinAIN1 = 9; //Direction
int pinAIN2 = 8; //Direction
int pinPWMA = 3; //Speed

//Motor 2
int pinBIN1 = 11; //Direction
int pinBIN2 = 12; //Direction
int pinPWMB = 5; //Speed

//Ping
int frontDistance  = 0;
int leftDistance   = 0;
int rightDistance  = 0;

boolean exploreMode;

//Standby
int pinSTBY = 10;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  //peizo sound
  pinMode(piezoPin, OUTPUT);

  //set pinMode
  pinMode(ledPin, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinSTBY, OUTPUT);

  //set exploreMode
  exploreMode = false;

  myServo.attach(servoPin);
  showMenu();
}

void loop()
{
  //explore();
  int incomingByte, i, n;
  Serial.flush();//flush all previous received and transmitted data
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    //flush buffer needed not sure buffer doesn't flush
   //while(Serial.available() > 0)
     // Serial.read();
    //There is an incoming command get out of exploreMode
    exploreMode = false;
    //stop the motors and go on standby
    brake();

    switch (incomingByte)
    {
      case 'b':
        Serial.println(F("Blink & Beep"));
        blink();
        beep();
        Serial.println(F("-----------"));
        break;
      case 'e':
        Serial.println(F("Explore"));
        exploreMode = true;
        Serial.println(F("-----------"));
        break;
      case 'i':
        Serial.println(F("System Information"));
        information();
        Serial.println(F("-----------"));
        break;
      case 't':
        Serial.println(F("System Telemetry"));
        telemetry();
        Serial.println(F("-----------"));
        break;
      case '?':
        showMenu();
        break;
      // Ignore some characters, like the carriage return and line feed.
      case '\'':
      case '\r':
      case '\n':
        break;
      default:
        Serial.print(F("Unknown command: 0x"));
        Serial.println(incomingByte,HEX);
        Serial.println(F("Type '?' followed by Enter"));
        // Something could be wrong, delete incoming data in buffer
        while (Serial.available() > 0)
          Serial.read();
        Serial.println(F("-----------"));
        break;
    }
  }
  digitalWrite(ledPin, exploreMode);
  if (exploreMode) explore();
}
/*============================== END OF FILE =================================*/

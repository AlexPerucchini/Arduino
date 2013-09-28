/* =========================================================================
This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse
   to return.  The length of the returning pulse is proportional to
   the distance of the object from the sensor.
   Then run the Sparkfun TB6612FNG 1A Dual Motor Driver using Arduino

This code conducts a few simple manuvers to illustrate the functions:
  - motorDrive(motorNumber, motorDirection, motorSpeed)
  - motorBrake(motorNumber)
  - motorStop(motorNumber)
  - motorsStandby

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

========================================================================= */

#include <Servo.h>

const int       servoPin = 2;
const int       pingPin  = 7;
const int       ledPin   = 13;
static boolean  turnCW   = 0;  //for motorDrive function
static boolean  turnCCW  = 1; //for motorDrive function
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
int distanceForward = 0;
int leftDistance    = 0;
int rightDistance   = 0;

//Standby
int pinSTBY = 10;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  //set pinMode
  pinMode(ledPin, OUTPUT);

  pinMode(pinPWMA, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinAIN2, OUTPUT);

  pinMode(pinPWMB, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);

  pinMode(pinSTBY, OUTPUT);

  myServo.attach(servoPin);
}

void loop()
{

  //get the distance(inches) from the ping function
  distanceForward = ping();
  Serial.print("distanceForward: ");
  Serial.println(distanceForward);

  //path is clear
  if (distanceForward > 8) {

    myServo.write(83); //center the servo
    goForward();
  }
  else if ((distanceForward > 4) && (distanceForward <= 9))
  {
    brake();
    //scan right
    myServo.write(110);
    delay(500);
    rightDistance = ping();
    Serial.print("rightDistance: ");
    Serial.println(rightDistance);
    delay(500);
    //scan left
    myServo.write(40);
    delay(500);
    leftDistance = ping();
    Serial.print("leftDistance: ");
    Serial.println(leftDistance);
    delay(500);
    myServo.write(83); //center the servo
    delay(500);
    compareDistance();
    blink();
  }
  else if (distanceForward <= 3)
  {
    goBackwards();
    blink();
  }

}
// ========================================================================

/* Ping distance functions */
void compareDistance()
{
  if (leftDistance > rightDistance)
  {
    turnLeft();
    Serial.println("turnLeft");
    delay(500);
  }
  else if(leftDistance < rightDistance)
  {
    turnRight();
    Serial.println("turnRight");
    delay(500);
  }
  else
  {
    reverse();
    delay(500);
  }

}

/* Ping functions */
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

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  //delay(50);

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

/* Motor functions */
void goForward()
{
  //Get the distance from the sensor and map it to achieve
  //a gradual increase to the motor speed
  int motorSpeed = ping();
  motorSpeed = map (motorSpeed, 8, 60, 100, 254);
  motorSpeed = constrain( motorSpeed, 0, 254);
  Serial.print("MotorSpeed: ");
  Serial.println(motorSpeed);
  //Drive both motors CW, full speed 255, half speed 127
  motorDrive(motor1, turnCW, motorSpeed);
  motorDrive(motor2, turnCW, motorSpeed);
}

void goBackwards()
{
  //Drive both motors CCW, full speed 255, half speed 127
  motorDrive(motor1, turnCCW, 220);
  motorDrive(motor2, turnCCW, 220);
}

void reverse()
{
  //Do a tight turn towards motor1: Motor2 forward, Motor1 reverse
  motorDrive(motor1, turnCCW, 192);
  motorDrive(motor2, turnCW, 192);
}

void turnLeft()
{
  //Turn towards motor1: Stop Motor1, slow Motor2
  motorStop(motor1);
  motorDrive(motor2, turnCW, 160);
}

void turnRight()
{
  //Turn towards motor2: Stop Motor2, slow Motor2
  motorStop(motor2);
  motorDrive(motor1, turnCW, 160);
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
/*
This "Short Brake"s the specified motor, by setting speed to zero
*/

  if (motorNumber == motor1)
    analogWrite(pinPWMA, 0);
  else
    analogWrite(pinPWMB, 0);

}


void motorStop(boolean motorNumber)
{
  /*
  This stops the specified motor by setting both IN pins to LOW
  */
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
  /*
  This puts the motors into Standby Mode
  */
  digitalWrite(pinSTBY, LOW);
}

/* Blink function */
void blink()
{
  //blink led 5 times
  for(int i= 0; i < 5; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
  }
}

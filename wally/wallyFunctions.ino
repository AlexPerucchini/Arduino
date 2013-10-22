/*
wallyFunctions.pde
Author: Alex Perucchini

This .pde file gets concatenated at the end of the Wally.ino file to separate
all functions calls from the main driver
*/

//=============================================================================
// Tell Wally to go exploring
//=============================================================================

void explore()
{
    int minSafeDist = 10;

    turnServoCenter();
    frontDistance = ping();

    if(frontDistance > minSafeDist)
    {
      goForward();
    }
    else
    {
      brake();
      turnServoRight();
      rightDistance = ping();
      turnServoLeft();
      leftDistance = ping();
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
      //escape
      goBackwards();
    }

}


//=============================================================================
// PING))) world and related functions
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
  //cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

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
  int motorSpeed = ping();
  motorSpeed = map (motorSpeed, 10, 60, 80, 200);
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
  beep();
  blink();
  delay(1000);
}

void reverse()
{
  //Do a tight turn towards motor1: Motor2 forward, Motor1 reverse
  motorDrive(motor1, turnCCW, 160);
  motorDrive(motor2, turnCW, 160);
  beep();
}

void turnLeft()
{
  //Turn towards motor1: Stop Motor1, slow Motor2
  motorStop(motor1);
  motorDrive(motor2, turnCW, 160);
  delay(800);
}

void turnRight()
{
  //Turn towards motor2: Stop Motor2, slow Motor2
  motorStop(motor2);
  motorDrive(motor1, turnCW, 160);
  delay(800);
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
  //blink led 5 times
  for(int i = 0; i < 5; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
  }
}

//=============================================================================
// Piezo beep
//=============================================================================

void beep(){
  //beep peizo 3 times
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(4, HIGH);
    delay(50);
    digitalWrite(4, LOW);
    delay(50);
  }
  blink();
}


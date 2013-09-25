const int BLED = 9;   //Blue LED
const int GLED = 10;  //Green LED
const int RLED = 11;  //Red LED
const int TEMP = 0;    //Temperature Sensor
const int LOWER_BOUND = 139;
const int UPPER_BOUND = 147;

int val = 0; //variable to hold analog reading


void setup()
{
  pinMode(BLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  Serial.begin(9600);
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

void loop()
{
  val = analogRead(TEMP);

  if(val > LOWER_BOUND)
  {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
    digitalWrite(BLED, HIGH);
  }
  else if(val < UPPER_BOUND)
  {
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, LOW);
    digitalWrite(BLED, LOW);
  }
  else
  {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, HIGH);
    digitalWrite(BLED, LOW);
  }


  Serial.print("temp sensor: ");
  Serial.println(val);

  //get the voltage
  float volts = calculate_voltage(val);
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
  delay(2000);

}

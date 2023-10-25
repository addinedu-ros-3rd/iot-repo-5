/*
  This program controls 2 DC motor attached to a body with a L298N motor driver.
  First, 2 wheels roll forwards.
  Second, 2 wheels roll backwards.
  Third, the body turns clockwise.
  Last, the body turns counterclockwise.

  The speed of the motor is controlled with ENA and ENB pin.
*/

// Motor Connections
#define ENA 10  // Right Side Motor
#define IN1 9
#define IN2 8
#define ENB 5  // Left Side Motor
#define IN3 7
#define IN4 6

void motorAccel()
{
  for (int i = 0; i < 256; i++) // i++
  {
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(20);
  }
}

void motorDecel()
{
  for (int i = 255; i >= 0; --i) // --i
  {
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(20);
  }
}

void setForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void setBackward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void setClockwise()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void setCounterclockwise()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start with motors off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  // Forward
  setForward();
  motorAccel();
  motorDecel();

  stopMotors();
  delay(300); 
  
  // Backward
  setBackward();
  motorAccel();
  motorDecel();
  
  stopMotors();
  delay(300); 
  
  // Turn Clockwise
  setClockwise();
  motorAccel();
  motorDecel();
  
  stopMotors();
  delay(300);
  
  // Turn Counterclockwise
  setCounterclockwise();
  motorAccel();
  motorDecel();
  
  stopMotors();
  delay(300);
}

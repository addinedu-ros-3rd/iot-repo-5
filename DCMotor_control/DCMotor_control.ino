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
#define ENB 4  // Left Side Motor
#define IN3 6
#define IN4 5

static const char goForward = 'w';
static const char goBackward = 's';
static const char turnLeft = 'a';
static const char turnRight = 'd';


void motorAccel() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}


void setForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void setBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void setClockwise() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void setCounterclockwise() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void setup() {
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
  char input[4] = {'a', 'd', 's', 'w'};

  Serial.begin(115200);

  while (Serial.available()) {
    char direction = Serial.read();

    if (sizeof(direction) == 1) {
      switch(direction) {
        // Forward
        case goForward:
          //Serial.println("going forward!");
          setForward();
          motorAccel();
          break;
        
        // Backward
        case goBackward:
          //Serial.println("going backward!");
          setBackward();
          break;
        
        // Turn Clockwise
        case turnLeft:
          //Serial.println("turning left!");
          setClockwise();
          motorAccel();
          break;
          
        // Turn Counterclockwise
        case turnRight:
          //Serial.println("turning right!");
          setCounterclockwise();
          motorAccel();
          break;
      }
    }
    else {
      Serial.println("Input options are [a, w, s, d]");
    }
  }
  delay(100);
  stopMotors();
  
}

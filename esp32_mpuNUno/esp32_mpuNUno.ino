#include <Wire.h>

// PIN SETUP

// MPU6050
// SCL 1 SDA 2

// UNO
// TX  17
// RX  18

#define MPU_SDA 2
#define MPU_SCL 1

// Motor Connections
#define ENA 10  // Right Side Motor
#define IN1 9
#define IN2 8
#define ENB 4  // Left Side Motor
#define IN3 6
#define IN4 5

// Motor Speed Rate (PWM)
const int minENA = 0;
const int maxENA = 255;
const int minENB = 0;
const int maxENB = 255;

static const char goForward = 'w';
static const char goBackward = 's';
static const char turnLeft = 'a';
static const char turnRight = 'd';
static const char returnChar = '\n'


const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  setupMPU();

  // Motor pin set up
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
  analogWrite(ENA, minENA);
  analogWrite(ENB, minENA);

  Serial.begin(115200);
  
  // wait for connection from pc
  while (!Serial);
  delay(500);
  Serial.println("PC Setup Complete");
  
  Serial1.begin(115200, SERIAL_8N1, 18, 17);  // RX TX
  Serial.println("Setup complete");
}


void loop() {

  printAccGyro();

  readFromUno();

  readFromPC();
  
  delay(100);
}


// while이라 uno로 보낼 때 딜레이 생김
void readFromUno() {
  while (Serial1.available()){
    char c = Serial1.read();
    Serial.print(c);
    if (c == '\n') {
      return;
    }
  }
}


void readFromPC() {
  while (Serial.available()){
    char direction = Serial.read();
    Serial1.print(direction);
    switch (direction) {
      case returnChar:
        return;
      case goForward:
        moveFoward();
      case goBackward:
        moveDown();
      case turnRight:
        moveRight();
      case turnLeft:
        moveLeft();
    }
    stopMotors();
  }
}

void motorAccel() {
  analogWrite(ENA, maxENA);
  analogWrite(ENB, maxENB);
}


void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}


void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  motorAccel();
}


void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  motorAccel();
}


void moveRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  motorAccel();
}


void moveLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  motorAccel();
}


void setupMPU() {
  Wire.begin(MPU_SDA, MPU_SCL, 100000); // sda, scl
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}


void printAccGyro() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  Serial.print("MPU ");
  Serial.print(AcX); Serial.print(",");
  Serial.print(AcY); Serial.print(",");
  Serial.print(AcZ); Serial.print(",");
  Serial.print(GyX); Serial.print(",");
  Serial.print(GyY); Serial.print(",");
  Serial.print(GyZ); Serial.print("\n");
}

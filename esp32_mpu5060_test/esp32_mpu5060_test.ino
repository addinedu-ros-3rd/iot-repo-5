#include <Wire.h>

"""
PIN SETUP

SCL 1
SDA 2
"""

TwoWire MPU = TwoWire(0);
TwoWire RFID = TwoWire(1);

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

const int MPU_SDA = 2;
const int MPU_SCL = 1;

void setup() {
  setupMPU();

  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {

  printAccGyro();
  delay(100);
}

void setupMPU() {
  MPU.begin(SDA_PIN, SCL_PIN, 100000); // sda, scl
  MPU.beginTransmission(MPU_ADDR);
  MPU.write(0x6B);  // PWR_MGMT_1 register
  MPU.write(0);     // set to zero (wakes up the MPU-6050)
  MPU.endTransmission(true);
}

void printAccGyro() {
  MPU.beginTransmission(MPU_ADDR);
  MPU.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  MPU.endTransmission(false);
  MPU.requestFrom(MPU_ADDR, 14, true); // request a total of 14 registers
  AcX = MPU.read() << 8 | MPU.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = MPU.read() << 8 | MPU.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = MPU.read() << 8 | MPU.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = MPU.read() << 8 | MPU.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = MPU.read() << 8 | MPU.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  GyY = MPU.read() << 8 | MPU.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  GyZ = MPU.read() << 8 | MPU.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)


  Serial.print(AcX); Serial.print(" , ");
  Serial.print(AcY); Serial.print(" , ");
  Serial.print(AcZ); Serial.print(" , ");
  Serial.print(GyX); Serial.print(" , ");
  Serial.print(GyY); Serial.print(" , ");
  Serial.print(GyZ); Serial.print("\n");
}
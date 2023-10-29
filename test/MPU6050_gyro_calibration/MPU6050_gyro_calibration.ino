#include <Wire.h>

float RateRoll, RatePitch, RateYaw;

// Declare the calibration variables
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;

void gyro_signals(void)
{
  // Start I2C communication with the gyro
  Wire.beginTransmission(0x68);  //0x68 == default address value of MPU6050 register

  // configure external Frame Synchronization (FSYNC) pin sampling
  // and the Digital Low Pass Filter (DLPF) setting for both the gyro and accelerometer
  Wire.write(0x1A);
  // setting external FSYNC pin sampling and DLPF to 10 Hz
  Wire.write(0x05);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  // set the sensitivity scale factor
  // trigger gyro self-test and configure the gyros' full scale range
  Wire.write(0x1B);
  // measurements of MPU6050 are recorded in LSB (Least Significant Bits)
  // +---------------------------------------------+
  // | FS_SEL | Full Scale Range | LSB Sensitivity |
  // |        |     (°/sec)      |   (LSB/°/sec)   |
  // +---------------------------------------------+
  // |    0   |     +/- 250      |       131       |
  // |    1   |     +/- 500      |       65.5      |
  // |    2   |     +/- 1000     |       32.8      |
  // |    3   |     +/- 2000     |       16.4      |
  // +---------------------------------------------+
  // binary converted to hexadecimal value as an address
  Wire.write(0x8);  // 0x08 == 65.5 LSB/deg/sec
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  // Access register storing gyro measurements
  Wire.write(0x43);
  Wire.endTransmission();

  // pull the information of the six registers from 43 to 48
  Wire.requestFrom(0x68, 6);  // (first register, request 6 bytes)
  // read the gyro measurements around the x, y, z-axis
  // the result of an unsigned 16-bit measurements
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read();
  int16_t GyroZ = Wire.read() << 8 | Wire.read();

  // convert the measure,ent units to deg/sec
  RateRoll = (float)GyroX / 65.5;
  RatePitch = (float)GyroY / 65.5;
  RateYaw = (float)GyroZ / 65.5;
}

void setup()
{
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  // set the clock speed of the I2C protocol to 400 kHz
  Wire.setClock(400000);
  Wire.begin();
  // time for MPU 6050 to start
  delay(250);
  
  Wire.beginTransmission(0x68);
  // start gyro in power mode
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  // Perform measurement calibration with 2000 read values
  // takes 2 seconds in total 
  for (RateCalibrationNumber = 0; RateCalibrationNumber < 2000; RateCalibrationNumber++)
  {
    gyro_signals();
    RateCalibrationRoll += RateRoll;
    RateCalibrationPitch += RatePitch;
    RateCalibrationYaw += RateYaw;
    delay(1); 
  }
  RateCalibrationRoll /= 2000;
  RateCalibrationPitch /= 2000;
  RateCalibrationYaw /= 2000;
}

void loop() {
  // read and display gyro measurements
  gyro_signals();
  RateRoll -= RateCalibrationRoll;
  RatePitch -= RateCalibrationPitch;
  RateYaw -= RateCalibrationYaw;

  Serial.print("Roll rate [°/s]= ");
  Serial.print(RateRoll);
  Serial.print("  Pitch rate [°/s]= ");
  Serial.print(RatePitch);
  Serial.print("  Yaw rate [°/s]= ");
  Serial.println(RateYaw);
  delay(50);
}

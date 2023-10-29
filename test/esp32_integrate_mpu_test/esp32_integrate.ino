#include <Wire.h>
#include <WiFi.h>

#include "MPU6050_6Axis_MotionApps20.h"


MPU6050 mpu;
 
#define INTERRUPT_PIN 14  // use pin 2 on Arduino Uno & most boards

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
VectorInt16 gy;         // [x, y, z]            gyro sensor measurements


volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

#define SETPOINT_PITCH_ANGLE_OFFSET 6.5   // CHANGE!


double setpointPitchAngle = SETPOINT_PITCH_ANGLE_OFFSET;
double pitchGyroAngle = 0;
//double pitchPIDOutput = 0;

double setpointYawRate = 0;
double yawGyroRate = 0;
//double yawPIDOutput = 0;
//
//#define PID_PITCH_KP 17000
//#define PID_PITCH_KI 0
//#define PID_PITCH_KD 0
//
//#define PID_YAW_KP 0.5
//#define PID_YAW_KI 0.5
//#define PID_YAW_KD 0
//
//PID pitchPID(&pitchGyroAngle, &pitchPIDOutput, &setpointPitchAngle, PID_PITCH_KP, PID_PITCH_KI, PID_PITCH_KD, DIRECT);
//PID yawPID(&yawGyroRate, &yawPIDOutput, &setpointYawRate, PID_YAW_KP, PID_YAW_KI, PID_YAW_KD, DIRECT);



// PIN SETUP

// MPU6050
// SCL 1 SDA 2

// UNO
// TX  17
// RX  18


const int MPUSCL = 1;
const int MPUSDA = 2;

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

static const String GO_FORWARD = "w";
static const String GO_BACKWARD = "s";
static const String TURN_LEFT = "a";
static const String TURN_RIGHT = "d";
static const String PAUSE = "p";

const char *ssid = "yourSSID";
const char *password = "yourPass";

WiFiServer server(80);
WiFiClient client;

String direction;
String rfid;
String mpu_data;

struct splitedStr {
  String str1;
  String str2;
};


// Declare the calibration variables
float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;



//=============================================
//=                   SETUP                   =
//=============================================
void setup() {

  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 18, 17);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  setupMPU();
  setupMotors();

  
  direction = "p";
  rfid = "None";
  mpu_data = "0/0/0";
}

void setupMPU() {

  Wire.begin(MPUSDA, MPUSCL);
  Wire.setClock(400000);

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
//           X Accel  Y Accel  Z Accel   X Gyro   Y Gyro   Z Gyro
//OFFSETS      2220,     -423,    1532,      70,     -15,    -121
  mpu.setXAccelOffset(2016); 
  mpu.setYAccelOffset(-427); 
  mpu.setZAccelOffset(1504);   
  mpu.setXGyroOffset(74);
  mpu.setYGyroOffset(-12);
  mpu.setZGyroOffset(-120);  
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) 
  {
      // Calibration Time: generate offsets and calibrate our MPU6050
      //mpu.CalibrateAccel(6);
      //mpu.CalibrateGyro(6);
      // turn on the DMP, now that it's ready
      mpu.setDMPEnabled(true);
      mpuIntStatus = mpu.getIntStatus();
      dmpReady = true;
      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
  } 
  else 
  {
      Serial.println("MPU Setup Failed");
  }
}

void setupMotors() {
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
}



//=============================================
//=                   LOOP                    =
//=============================================
void loop() {
  String s;
  client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
 
    while (client.connected()) {
      if (client.available()) {
        s = client.readStringUntil('\n');
        funcByInput(s);
      }

      readFromUno();
      mpu_data = getAccGyro();

      String sendToPC = "MOVE ";
      sendToPC += direction;
      sendToPC += ",RFID ";
      sendToPC += rfid;
      sendToPC += ",MPU ";
      sendToPC += mpu_data;
      client.println(sendToPC);

      direction = "p";
      rfid = "None";
      mpu_data = "0/0/0";

      delay(100);
    }
    Serial.println("Client Disconnected.");
  }

}



void funcByInput(String s) {
  struct splitedStr tmp = splitByTwo(s, ',');

  String moveLine = tmp.str1;
  String userLine = tmp.str2;

  tmp = splitByTwo(moveLine, ' ');
  String moveOper = tmp.str1;
  String moveType = tmp.str2;
//  Serial.println(moveOper);
//  Serial.println(moveType);
  direction = moveType;

  tmp = splitByTwo(userLine, ' ');
  String userOper = tmp.str1;
  String userType = tmp.str2;
//  Serial.println(userOper);
//  Serial.println(userType);

  moveCar();
  // send user of rfid to uno function here
}

struct splitedStr splitByTwo(String s, char sep) {
  int sepIndex = -1;
  String sCopy = s;

  sepIndex = s.indexOf(sep);
  struct splitedStr tmp;

  if (sepIndex >= 0) {
    tmp.str1 = sCopy.substring(0, sepIndex);
    tmp.str2 = sCopy.substring(sepIndex + 1, sizeof(s));
  }

  return tmp;
}

// while이라 uno로 보낼 때 딜레이 생김
void readFromUno() {
  if (Serial1.available()) {
    struct splitedStr tmp;
    String s = Serial1.readStringUntil('\n');
    tmp = splitByTwo(s, ' ');
    rfid = tmp.str2;
    Serial.println(rfid);
  }
  else {
    rfid = "None";
//    Serial.println(rfid);
  }
}



//=============================================
//=                  Motors                   =
//=============================================
void moveCar() {
  if (direction == PAUSE) {
//    Serial.println("STOP");
    stopMotors();
  }
  else if (direction == GO_FORWARD) {
    Serial.println("Move Forward");
    moveForward();
  }
  else if (direction == GO_BACKWARD) {
    Serial.println("Move Backward");
    moveBackward();
  }
  else if (direction == TURN_RIGHT) {
    Serial.println("Turn right");
    moveRight();
  }
  else if (direction == TURN_LEFT) {
    Serial.println("Turn Left");
    moveLeft();
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

String getAccGyro() {
  String tmp = "";
  
  Serial.println("GetGyro");
  
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) 
  {  
//    mpu.dmpGetQuaternion(&q, fifoBuffer);
//    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
//    mpu.dmpGetGyro(&gy, fifoBuffer);

//    yawGyroRate = gy.z;                   //rotation rate in degrees per second
    pitchGyroAngle = ypr[1] * 180/M_PI;   //angle in degree

    // Yaw/Pitch/Roll
    tmp += ypr[0]; tmp += "/";
    tmp += ypr[1]; tmp += "/";
    tmp += ypr[2];
  }
  else {
    tmp = "-0/-0/-0";
  }
  
  return tmp;
}

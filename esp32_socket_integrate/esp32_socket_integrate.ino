#include <Wire.h>
#include <WiFi.h>


// PIN SETUP

// MPU6050
// SCL 1 SDA 2

// UNO
// TX  17
// RX  18


// MPU Settings
#define MPU_SDA 2
#define MPU_SCL 1

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

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
  mpu_data = "-1,-1,-1,-1,-1,-1";
}

void setupMPU() {
  Wire.begin(MPU_SDA, MPU_SCL, 100000); // sda, scl
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
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
      mpu_data = "-1,-1,-1,-1,-1,-1";

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

  String tmp = "";
  tmp += AcX; tmp += ",";
  tmp += AcY; tmp += ",";
  tmp += AcZ; tmp += ",";
  tmp += GyX; tmp += ",";
  tmp += GyY; tmp += ",";
  tmp += GyZ;

  return tmp;
}

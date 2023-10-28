/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */
#include <Wire.h>
#include <WiFi.h>

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

static const char GO_FORWARD = 'w';
static const char GO_BACKWARD = 's';
static const char TURN_LEFT = 'a';
static const char TURN_RIGHT = 'd';
static const char RETURN_CHAR = '\n';

char direction;


const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);
WiFiClient client;

void setup()
{
  setupMPU();
  setupMotors();
  
  Serial.begin(115200);
  Serial1.begin(115200);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  
  Serial.println("Server started");
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


void loop(){
 client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    direction = '\n';
    while (client.connected()) {
      if (client.available()) {
        String s = client.readStringUntil('\n');
        Serial.println(s);
      }

      client.print("Move ");
      client.println(direction);
      Serial.print("Move ");
      Serial.println(direction);
          
      direction = '\n';
      readFromUno();

      moveCar(direction);
//        direction = checkEnd(currentLine); 
     
      delay(100);
    }
    Serial.println("Client Disconnected.");
  }
}

char checkEnd(String line) {
  // Check to see if the client request was "GET /H" or "GET /L":
  if (line.endsWith("GET /w")) {
    moveForward();
  }
  else if (line.endsWith("GET /a")) {
    moveRight();
  }
  else if (line.endsWith("GET /s")) {
    moveBackward();
  }
  else if (line.endsWith("GET /d")) {
    moveLeft();
  }
  else {
    stopMotors();
  }
}

void moveCar(char direction) {
  // print datas here
  printAccGyro();
  switch (direction) {
    case RETURN_CHAR:
      stopMotors();
      break;
    case GO_FORWARD:
      moveForward();
      break;
    case GO_BACKWARD:
      moveBackward();
      break;
    case TURN_RIGHT:
      moveRight();
      break;
    case TURN_LEFT:
      moveLeft();
      break;
  }
  stopMotors();
}


// while이라 uno로 보낼 때 딜레이 생김
void readFromUno() {
  while (Serial1.available()){
    Serial.print("here is rfid");
    char c = Serial1.read();
    client.print(c);
    if (c == '\n') {
      return;
    }
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

  client.print("MPU ");
  client.print(AcX); client.print(",");
  client.print(AcY); client.print(",");
  client.print(AcZ); client.print(",");
  client.print(GyX); client.print(",");
  client.print(GyY); client.print(",");
  client.print(GyZ); client.print("\n");
}

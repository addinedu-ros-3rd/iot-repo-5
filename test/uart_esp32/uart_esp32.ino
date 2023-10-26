// PIN SETUP

// MPU6050
// SCL 1 SDA 2

// UNO
// TX  17
// RX  18


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 18, 17);  // RX TX
}

void loop() {
  if (Serial1.available()){
    Serial.println("Messaggio ricevuto:");
    Serial.println(Serial1.readStringUntil('\n'));
  }
}
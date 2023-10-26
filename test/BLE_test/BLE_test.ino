#include <ArduinoBLE.h>

int cnt = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!BLE.begin()) {
    Serial.println("BLE Start Failed.");
    while(1);
  }
  BLE.setLocalName("ESP32-S3");
  BLE.advertise();
  Serial.println("Bluetooth Device Active. Waiting for Connections...");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();

  if(central) {
    Serial.print("Connected to CEntral: ");
    Serial.println(central.address());
    while(central.connected()) {
      Serial.println(cnt++);
      delay(1000);
    }
  }
}

#include <SPI.h>
#include <MFRC522.h>

#define RFID_SS_PIN 10
#define RFID_RST_PIN 4
 
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

// Init array that will store new NUID 
byte nuidPICC[4];

int i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  scanRFID();

  if (Serial.available() > 0){
    String input = Serial.readStringUntil('\n');

    if (!input.equals("")) {
      Serial.print("input read, ");
      Serial.println(input);
    }
  }
  Serial.println(i++);
  delay(1000/100);
}

void scanRFID() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent()){
    return;
  }
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) 
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }
  
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    
    Serial.print(F("RFID Detected "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else return;

  // Serial.println(rfid.MIFARE_GetValue((byte *)rfid.uid.uidByte, (int32_t *)rfid.uid.size));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
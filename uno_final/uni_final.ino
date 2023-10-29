#include <SPI.h>
#include <MFRC522.h>

//PIN SETUP
//RST   9
//SDA   10
//SCK   13
//MOSI  11
//MISO  12

#define RST_PIN     9 // any available GPIO          
#define SS_PIN      10 // SPI_CS -> GPIO 10
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

// Init array that will store new NUID 
byte nuidPICC[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  String inputStr = readFromESP();
  scanRFID();
  delay(100);
}


String readFromESP() {
  if (Serial.available()){
    return Serial.readStringUntil('\n');
  }
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
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
  }
  
  printHex(rfid.uid.uidByte, rfid.uid.size);
  
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  Serial.print("RFID ");
  for (byte i = 0; i < bufferSize; i++) {
    if (i != 0) {
      Serial.print(":");  
    }
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

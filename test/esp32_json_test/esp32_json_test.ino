#include <Wire.h>
#include <WiFi.h>


// PIN SETUP

// MPU6050
// SCL 1 SDA 2

// UNO
// TX  17
// RX  18


const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);
WiFiClient client;

char direction;
String rfid;

struct splitedStr {
  String str1;
  String str2;  
};

void setup() {
  
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 18, 17);
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

void loop() {
  String s;
  client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    direction = 'p';
    while (client.connected()) {
      if (client.available()) {
        s = client.readStringUntil('\n');
        funcByInput(s);
      }

      readFromUno();
      
      String sendToPC = "MOVE ";
      sendToPC += String(direction);
      sendToPC += ",RFID ";
      sendToPC += rfid;
      sendToPC += ",MPU ";
      sendToPC += "-1,-1,-1,-1,-1,-1";
      client.println(sendToPC);
      Serial.println(sendToPC);
      
      direction = 'p';
      rfid = "None";
      
//      Serial.print("Move "); Serial.println(direction);
          
//      direction = '\n';
      

//      moveCar(direction);
//        direction = checkEnd(currentLine); 
     
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

  tmp = splitByTwo(userLine, ' ');
  String userOper = tmp.str1;
  String userType = tmp.str2;

  Serial.println(moveOper);  
  Serial.println(moveType);
  Serial.println(userOper);
  Serial.println(userType);
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
  if (Serial1.available()){
    struct splitedStr tmp;
    String s = Serial1.readStringUntil('\n');
    tmp = splitByTwo(s, ' ');
    rfid = tmp.str2;
    Serial.println(rfid);
  }
  else {
    rfid = "None";
    Serial.println(rfid);
  }
}

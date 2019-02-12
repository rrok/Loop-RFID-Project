#include <Homie.h>
#include "MFRC522.h"
String uid;
String data="";
#define RST_PIN  8  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
HomieNode rfidNode("READER", "CPR");
unsigned long previousMillis = 0;
const int interval = 2000;
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();

void setupHandler() {
  Serial.begin(9600);    // Initialize serial communications
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
}
// Global Timer

void dump_byte_array(byte *buffer, byte bufferSize) {
  String uid;
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], HEX);
    
    uid = uid + String(buffer[i], HEX);
  
  }
    if(uid){
      Homie.getLogger()<<"uid Found"<< uid <<endl;
      JSONencoder["sensorType"] = "READER RFID";
      JSONencoder["companyID"] = "CEDI-01";
      JSONencoder["companyName"] = "Commercianti Indipendenti Associati Soc. Coop.";
      JSONencoder["address"] = "Via dei Mercanti, 3, 47122 Forlì FC, Italia";
      JSONencoder["temperature"] = "23";
      JSONencoder["lat"] = "44.2347418";
      JSONencoder["lon"] = "12.0612333";
      JSONencoder["boxUID"] = uid;
      char JSONmessageBuffer[300];
      JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      //rfidNode.setProperty(uid).send(uid);
      Serial.print(F("\nCard UID:"));
      Serial.print(uid);
      rfidNode.setProperty("data").send("JSONmessageBuffer");
      
  }
}

void loopHandler() {

   if ( ! mfrc522.PICC_IsNewCardPresent()) {
    //Serial.print("scanning");
    delay(50);
    return;
  }
  Homie.getLogger()<<"Card Present" <<endl;
  // scan the cards. Put in a non-blocking delay to avoid duplicate readings
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // do non-blocking thing here

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        Homie.getLogger()<<"reading" <<endl;
      //Serial.print("found card...");
      delay(50);
      return;
    }

    // Process card
    //Serial.print(F("\nCard UID:"));
    Homie.getLogger()<<"card information" <<endl;
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    }
}

void setup() {
  Serial.begin(9600);
  Serial << endl << endl;
  Homie_setFirmware("READER", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);
  Homie.setLoggingPrinter(&Serial);
  Homie.setup();

}

void loop() {
  Homie.loop();
}
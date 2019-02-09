#include <Homie.h>
#include "MFRC522.h"
const int TEMPERATURE_INTERVAL = 300;
unsigned long lastTemperatureSent = 0;
HomieNode temperatureNode("temperature", "temperature");


String uid;
#define RST_PIN  9  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
HomieNode rfidNode("RFID-1", "sensor");
unsigned long previousMillis = 0;
const int interval = 2000;

void setupHandler() {
  Serial.begin(9600);    // Initialize serial communications
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  temperatureNode.setProperty("unit").send("c");
  rfidNode.setProperty("uid").send("id");
}
// Global Timer

void dump_byte_array(byte *buffer, byte bufferSize) {
  String uid;
  const long interval = 1000;

  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    uid = uid + String(buffer[i], HEX);
  
  }
    if(uid){
     //6Homie.setProperty(rfidNode, "uid", uid, true);
     rfidNode.setProperty(uid).send(uid);
  }
}

void loopHandler() {

   if ( ! mfrc522.PICC_IsNewCardPresent()) {
    //Serial.print("scanning");
    delay(50);
    return;
  }

  // scan the cards. Put in a non-blocking delay to avoid duplicate readings
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // do non-blocking thing here

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      Serial.print("found card...");
      delay(50);
      return;
    }

    // Process card
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    }
}

void setup() {
  Serial.begin(9600);
  Serial << endl << endl;
  Homie_setFirmware("rfid", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);
  Homie.setup();

}

void loop() {
  Homie.loop();
}
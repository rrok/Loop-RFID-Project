#include <Arduino.h>
#include <Homie.h>
void setup() {
  Serial.begin(9600);
  Serial << endl << endl;
  //efrecon/mqtt-client pub -h localhost -t "mytest" -m "ok"
  Homie_setFirmware("bare-minimum", "1.0.0"); // The underscore is not a typo! See Magic bytes
  Homie.setup();
}

void loop() {
  Homie.loop();
}
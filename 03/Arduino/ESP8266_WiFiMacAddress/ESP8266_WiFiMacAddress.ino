#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.print("\nMAC address = ");
  Serial.println(WiFi.macAddress());
}

void loop() {}

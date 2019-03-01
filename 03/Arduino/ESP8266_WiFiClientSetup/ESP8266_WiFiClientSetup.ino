#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to Wi-Fi ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); // or WIFI_AP, WIFI_AP_STA, WIFI_OFF
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // keeps watchdog happy
  }
  Serial.print("Connected, IP = "); 
  Serial.println(WiFi.localIP());
}

void loop() {}

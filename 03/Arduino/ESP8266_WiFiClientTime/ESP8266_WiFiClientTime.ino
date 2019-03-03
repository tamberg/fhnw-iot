#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // keeps watchdog happy
  }
  Serial.print("Connected, IP = "); 
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  // http://www.esp8266.com/viewtopic.php?f=19&t=809
  if (client.connect("google.com", 80)) {
    // write HTTP request:
    client.print("HEAD / HTTP/1.1\r\n" \
      "Host: google.com\r\n" \
      "Connection: close\r\n\r\n");
    // read HTTP response:
    client.find("Date: ");
    // e.g. "Sat, 02 Mar 2019 13:37:00"
    int n = 16 + 1 + 8;
    while (n > 0) {
      Serial.print((char) client.read());
      n--;
    }
    Serial.println();
    client.find("\r\n\r\n");
    //client.stop();
  }
}

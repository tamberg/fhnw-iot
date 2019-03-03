#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

const char *host = "www.howsmyssl.com"; // via @spiessa
const char *fingerprint = // SHA-1, not very secure anymore
  "67 FF 3D BC D3 59 E2 C0 BD 04 3B 8A 57 CB 72 7C 0A 20 F5 E4"; 
const char *path = "/a/check";
const int port = 443;

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // keeps watchdog happy
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());

  // connect to remote host
  WiFiClientSecure client; // use TLS
  if (client.connect(host, port) &&
    client.verify(fingerprint, host)) {

    // send HTTPS request
    client.print("GET ");
    client.print(path);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(host);
    client.print("\r\n");
    client.print("Connection: close\r\n\r\n");

    // read HTTPS response
    while (client.connected() || client.available()) {
      int ch = client.read();
      while (ch >= 0) {
          Serial.print((char) ch);
          ch = client.read();
      }
    }
  }
}

void loop() {}

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

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
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());

//  const char *host = "api.github.com";
//  const char *path = "/repos/esp8266/Arduino/commits/master/status";
  const char *host = "www.howsmyssl.com";
  const char *path = "/a/check";
  const int port = 443;

  // connect to remote host
  WiFiClientSecure client;
  if (client.connect(host, port)) {

    // send HTTP request
    client.print("GET ");
    client.print(path);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(host);
    client.print("\r\n");
//    client.print("User-Agent: BuildFailureDetectorESP8266\r\n");
    client.print("Connection: close\r\n\r\n");

    // read HTTP response
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

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
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());

  const char *host = "tmb.gr";
  const char *path = "/hello.html";
  const int port = 80;

  // connect to remote host
  WiFiClient client;
  if (client.connect(host, port)) {

    // send HTTP request
    client.print("GET ");
    client.print(path);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(host);
    client.print("\r\n");
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

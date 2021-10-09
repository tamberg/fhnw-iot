#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; 
const char *password = "MY_PASSWORD";
const int port = 80;

WiFiServer server(port);

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client && client.connected()) {
    Serial.println("Connection accepted, remote IP = ");
    Serial.println(client.remoteIP());

    // Read HTTP request
    int ch = client.read();
    while (ch != -1) {
      Serial.print((char) ch);
      ch = client.read();
    }

    // Send HTTP response
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Length: 9\r\n");
    client.print("Connection: close\r\n");
    client.print("\r\n");
    client.print("It works!");

    delay(1); // Give Web client time to receive data
    client.stop();
  }
}

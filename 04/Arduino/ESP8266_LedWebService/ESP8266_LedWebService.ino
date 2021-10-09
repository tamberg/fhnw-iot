#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; 
const char *password = "MY_PASSWORD";
const int ledPin = 5; // Grove adapter I2C_1 or _2 used as D6
const int port = 80;

WiFiServer server(port);

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());
  server.begin();
}

void sendResponse(WiFiClient client) {
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Length: 0\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");
}

void loop() {
  WiFiClient client = server.available();
  if (client && client.connected()) {
    Serial.println(client.remoteIP());
    // PUT /led/state/1
    // PUT /led?state=1
    if (client.find("state")) {
      int state = client.parseInt();
      state = min(max(state, 0), 1);
      Serial.println(state);
      digitalWrite(ledPin, state ? HIGH : LOW);
    }
    client.find("\r\n\r\n"); // Consume incoming request
    sendResponse(client);
    delay(1); // Give Web browser time to receive data
    client.stop();
  }
}

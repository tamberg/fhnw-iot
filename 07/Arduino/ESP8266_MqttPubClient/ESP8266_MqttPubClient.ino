#include <ESP8266WiFi.h>
#include <ESP8266MQTTClient.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

MQTTClient client;
volatile int connected = 0;

void handleConnected() {
  Serial.println("Connected to broker");
  connected = 1;
}

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

  client.onConnect(handleConnected);
  client.begin("mqtt://test.mosquitto.org/");
}

void loop() {
  client.handle();
  if (connected) {
    float temp = analogRead(A0) / 10.0f; // TODO
    String tempStr = String(temp, 2);
    Serial.println(tempStr);
    client.publish("temp", tempStr);
    delay(1000);
  }
}

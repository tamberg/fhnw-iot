#include <ESP8266WiFi.h>
#include <ESP8266MQTTClient.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

MQTTClient client;

void handleConnected() {
  Serial.println("Connected to broker");
  client.subscribe("hello");
}

void handleSubscribed(int topicId) {
  Serial.println("Subscribed");
}

void handleDataReceived(String topic, String data, bool b) {
  Serial.print("Received topic: ");
  Serial.print(topic);
  Serial.print(", data: ");
  Serial.println(data);
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
  client.onSubscribe(handleSubscribed);
  client.onData(handleDataReceived);
  client.begin("mqtt://test.mosquitto.org/");
}

void loop() {
  client.handle();
}

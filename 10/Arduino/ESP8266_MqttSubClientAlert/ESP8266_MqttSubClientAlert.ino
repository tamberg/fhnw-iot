#include <ESP8266WiFi.h> // v2.4.2
#include <ESP8266MQTTClient.h> // v1.0.4

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO
const char *host = "mqtt://LOCAL_IP"; // TODO
const int buzzerPin = 5;

MQTTClient client;

void handleConnected() {
  Serial.println("Connected to broker");
  client.subscribe("alert");
}

void handleSubscribed(int topicId) {
  Serial.println("Subscribed");
}

void handleDataReceived(String topic, String data, bool b) {
  Serial.print("Received topic: ");
  Serial.print(topic);
  Serial.print(", data: ");
  Serial.println(data);
  if (data == "on") {  
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
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
  client.begin(host);
}

void loop() {
  client.handle();
}

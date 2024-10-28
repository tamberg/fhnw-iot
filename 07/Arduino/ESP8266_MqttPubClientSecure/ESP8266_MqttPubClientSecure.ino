#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define MQTT_CONN_KEEPALIVE 30

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO
const char *host = "test.mosquitto.org";
const int port = 8883;
const char *topicStr = "hello";

BearSSL::WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client, host, port);

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
  client.setInsecure(); // no cert validation
}

void loop() {
  if (mqtt.connected()) {
    float temp = analogRead(A0) / 10.0f; // TODO
    String payloadStr = String(temp, 2);
    Serial.println(payloadStr);
    mqtt.publish(topicStr, payloadStr.c_str(), sizeof(payloadStr));
    delay(1);
  } else {
    int result = mqtt.connect(); // calls client.connect()
    if (result != 0) {
      Serial.println(mqtt.connectErrorString(result));
      delay(3000);
    }
  }
}
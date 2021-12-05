#include <ESP8266WiFi.h> // v2.4.2
#include <ESP8266MQTTClient.h> // v1.0.4
#include "DHTesp.h"

const int dhtPin = 5; // Grove adapter I2C_1 or _2 used as D6
const DHTesp::DHT_MODEL_t dhtModel = DHTesp::DHT11;

DHTesp dht;

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO
const char *host = "mqtt://LOCAL_IP"; // TODO

MQTTClient client;
volatile int connected = 0;

void handleConnected() {
  Serial.println("Connected to broker");
  connected = 1;
}

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin, dhtModel);
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
  client.begin(host);
}

void loop() {
  client.handle();
  if (connected) {
    // Readings take about 250 ms, may be up to 2 s old
    float temp = dht.getTemperature(); // *C
    if (!isnan(temp)) {
      String tempStr = String(temp, 2);
      Serial.println(tempStr);
      client.publish("temp", tempStr);
    }
    delay(1000);
  }
}

// Test code for Grove DHT11 humidity/temperature sensor on ESP8266

// https://github.com/tamberg/fhnw-iot/wiki/Grove-Sensors#temperature--humidity-sensor-dht11
// https://github.com/tamberg/fhnw-iot/wiki/Feather-Huzzah-ESP8266
// https://github.com/tamberg/fhnw-iot/wiki/Grove-Adapters#pinout

#include "DHTesp.h"

#define DHT_PIN 5 // Grove adapter I2C_1 or _2 used as D6
#define DHT_MODEL DHTesp::DHT11

DHTesp dht;

void setup() {
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHT_MODEL);
}

void loop() {
  float humi = dht.getHumidity(); // %
  float temp = dht.getTemperature(); // *C

  if (isnan(humi) || isnan(temp)) {
    Serial.print("Failed to read from DHT11 sensor");
  } else {
    Serial.print(humi, 1);
    Serial.print(" %, ");
    Serial.print(temp, 1);
    Serial.print(" *C\n");
  }

  delay(dht.getMinimumSamplingPeriod() + 200); // ms
}

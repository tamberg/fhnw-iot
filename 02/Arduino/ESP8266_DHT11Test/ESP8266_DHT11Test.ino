// Test code for Grove DHT11 humidity/temperature sensor on ESP8266

// https://github.com/tamberg/fhnw-iot/wiki/Grove-Sensors#temperature--humidity-sensor-dht11
// https://github.com/tamberg/fhnw-iot/wiki/Feather-Huzzah-ESP8266
// https://github.com/tamberg/fhnw-iot/wiki/Grove-Adapters#pinout

#include "DHTesp.h"

const int dhtPin = 5; // Grove adapter I2C_1 or _2 used as D6
const DHTesp::DHT_MODEL_t dhtModel = DHTesp::DHT11;

DHTesp dht;

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin, dhtModel);
}

void loop() {
  // Readings take about 250 ms, may be up to 2 s old
  float humi = dht.getHumidity(); // %
  float temp = dht.getTemperature(); // *C

  if (isnan(humi) || isnan(temp)) {
    Serial.print("Failed to read from DHT11 sensor\n");
  } else {
    Serial.print(humi, 1);
    Serial.print(" %, ");
    Serial.print(temp, 1);
    Serial.print(" *C\n");
  }

  delay(dht.getMinimumSamplingPeriod() + 200); // ms
}

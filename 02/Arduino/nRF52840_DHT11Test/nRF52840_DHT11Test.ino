// Test code for Grove DHT11 humidity/temperature sensor on nRF52840

// https://github.com/tamberg/fhnw-iot/wiki/Grove-Sensors#temperature--humidity-sensor-dht11
// https://github.com/tamberg/fhnw-iot/wiki/Feather-nRF52840-Express
// https://github.com/tamberg/fhnw-iot/wiki/Grove-Adapters#pinout

// Based on example code written by ladyada, public domain

#include "DHT.h"

#define DHT_PIN 5 // Grove adapter D2
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // Readings take about 250 ms, may be up to 2 s old
  float humi = dht.readHumidity(); // %
  float temp = dht.readTemperature(); // *C

  if (isnan(humi) || isnan(temp)) {
    Serial.print("Failed to read from DHT11 sensor\n");
  } else {
    Serial.print(humi, 1);
    Serial.print(" %, ");
    Serial.print(temp, 1);
    Serial.print(" *C\n");
  }

  delay(2000); // ms
}

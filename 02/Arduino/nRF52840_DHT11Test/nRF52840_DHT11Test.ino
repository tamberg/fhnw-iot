// Test code for Grove DHT11 humidity/temperature sensor on nRF52840

// https://github.com/tamberg/fhnw-iot/wiki/Grove-Sensors#temperature--humidity-sensor-dht11
// https://github.com/tamberg/fhnw-iot/wiki/Feather-nRF52840-Express
// https://github.com/tamberg/fhnw-iot/wiki/Grove-Adapters#pinout

// Based on example code written by ladyada, public domain

#include "Adafruit_TinyUSB.h" // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653
#include "DHT.h"

const int dhtPin = 5; // Grove adapter D2
const int dhtType = DHT11;

DHT dht(dhtPin, dhtType);

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

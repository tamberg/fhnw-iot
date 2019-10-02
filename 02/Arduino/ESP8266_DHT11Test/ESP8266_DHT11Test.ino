#include "DHTesp.h"

const int dhtPin = 5; // Grove adapter I2C_1 or _2 used as D6
const DHTesp::DHT_MODEL_t dhtType = DHTesp::DHT11;

DHTesp dht;

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin, dhtType);
}

void loop() {
  float humi = dht.getHumidity();
  float temp = dht.getTemperature();

  Serial.print(millis());
  Serial.print(", ");
  Serial.print(dht.getStatusString());
  Serial.print(", ");
  Serial.print(humi, 1);
  Serial.print(", ");
  Serial.print(temp, 1);
  Serial.print("\n");

  delay(dht.getMinimumSamplingPeriod() + 200);
}

#include "Adafruit_TinyUSB.h" // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653

int buttonPin = 9; // Grove D4
int ledPin = 5; // Grove D2

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
}

void loop() {
  int state = digitalRead(buttonPin);
  Serial.println(state);
  //if (state == HIGH) {
  //  digitalWrite(ledPin, HIGH); // on
  //} else {
  //  digitalWrite(ledPin, LOW); // off
  //}
  digitalWrite(ledPin, state);
  delay(1);
}

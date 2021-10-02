#include "Adafruit_TinyUSB.h"; // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653

int buttonPin = 9; // Grove D4
int ledPin = 5; // Grove D2

int s = 0; // state
long t0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
}

void loop() {
  int b = digitalRead(buttonPin);
  Serial.println(b);
  if (s == 0 && b == HIGH) {
    s = 1;
    digitalWrite(ledPin, HIGH); // on
  } else if (s == 1 && b == LOW) {
    s = 2;
  } else if (s == 2 && b == HIGH) {
    s = 3;
    t0 = millis();
  } else if (s == 3 && b == HIGH && (millis() - t0) >= 1000) {
    s = 4;
    digitalWrite(ledPin, LOW); // off
  } else if (s == 3 && b == LOW) {
    s = 2;
  } else if (s == 4 && b == LOW) {
    s = 0;
  }
  delay(1);
}

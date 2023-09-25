#include "Adafruit_TinyUSB.h"; // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653

int buttonPin = 9; // Grove D4
int ledPin = 5; // Grove D2

int s = 0; // state
long t0;

int pressed(int val) {
  return val == HIGH;
}

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
}

void loop() {
  int b = digitalRead(buttonPin);
  Serial.println(b);
  if (s == 0 && pressed(b)) {
    digitalWrite(ledPin, HIGH); // on
    s = 1;
  } else if (s == 1 && !pressed(b)) {
    s = 2;
  } else if (s == 2 && pressed(b)) {
    t0 = millis();
    s = 3;
  } else if (s == 3 && pressed(b) && (millis() - t0) >= 1000) {
    digitalWrite(ledPin, LOW); // off
    s = 4;
  } else if (s == 3 && !pressed(b)) {
    s = 2;
  } else if (s == 4 && !pressed(b)) {
    s = 0;
  }
  delay(1);
}

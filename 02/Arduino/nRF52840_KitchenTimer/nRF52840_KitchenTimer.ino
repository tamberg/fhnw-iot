#include "Adafruit_TinyUSB.h"; // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653
#include <TM1637.h>

int tm1637Clk = 9; // Grove D4
int tm1637Dio = 10; // Grove D5
int rotaryPin = A2; // Grove A2
int buttonPin = A4; // Grove A4
int buzzerPin = 5; // Grove D2

int state = 0;
int dt; // sec
long t0; // ms

TM1637 tm1637(tm1637Clk, tm1637Dio);

void setup() {
  Serial.begin(115200);
  pinMode(rotaryPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  tm1637.set(BRIGHT_TYPICAL); // or BRIGHT_DARKEST, ...
  tm1637.init();
  tm1637.point(POINT_ON); // or POINT_OFF
  tm1637.display(0, 0);
  tm1637.display(1, 0);
  tm1637.display(2, 0);
  tm1637.display(3, 0);
}

int pressed(int value) {
  return value == HIGH; // inverted
}

void display_time(long t) { // sec
  int m = t / 60;
  int m1 = m / 10;
  int m0 = m % 10;
  int s = t % 60;
  int s1 = s / 10;
  int s0 = s % 10;
  tm1637.display(0, m1);
  tm1637.display(1, m0);
  tm1637.display(2, s1);
  tm1637.display(3, s0);
  Serial.printf("%d => %d%d:%d%d\n", t, m1, m0, s1, s0);
}

void loop() {
  int r = analogRead(rotaryPin);
  int b = digitalRead(buttonPin);
  long t = millis();
  Serial.printf("state = %d, r = %d, b = %d\n", state, r, b);
  if (state == 0 && !pressed(b)) { // set
    dt = map(r, 0, 1024, 180, 0);
    display_time(dt);
  } else if (state == 0 && pressed(b)) { // start
    t0 = t;
    state = 1; // counting
  } else if (state == 1 && pressed(b)) { // cancel
    state = 0; // setting
  } else if (state == 1 && !pressed(b) && ((t - t0) / 1000) >= dt) { // alert
    display_time(0);
    digitalWrite(buzzerPin, HIGH);
    state = 2; // alerting
  } else if (state == 1 && !pressed(b)) { // display time
    display_time(dt - ((t - t0) / 1000));
  } else if (state == 2 && pressed(b)) { // confirm alert
    digitalWrite(buzzerPin, LOW);
    state = 0; // setting
  }
  delay(100);
}

int buttonPin = 9; // Grove D4
int ledPin = 5; // Grove D2

int s = 0; // state

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
    digitalWrite(ledPin, LOW); // off
  } else if (s == 3 && b == LOW) {
    s = 0;
  }
  delay(1);
}

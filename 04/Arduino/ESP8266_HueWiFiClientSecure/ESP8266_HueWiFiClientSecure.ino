// https://developers.meethue.com/develop/hue-api/
// $ curl -vX PUT https://HUE_BRIDGE_HOST/api/newdeveloper/lights/2/state -d '{"on":false}'

#include <ESP8266WiFi.h>

const int buttonPin = 5; // Grove adapter I2C_1 or _2 used as D6
const int ledPin = 0; // Built-in LED

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

const char *host = "HUE_BRIDGE_HOST"; // TODO
const int port = 443;

int buttonState = 0;

void sendHueStateWebRequest(int lightId, char *content) {
  const char *verb = "PUT";
  const char *pathPrefix = "/api/newdeveloper/lights/";
  const char *pathPostfix = "/state";

  // connect to remote host
  BearSSL::WiFiClientSecure client; // use TLS
  client.setInsecure(); // no cert validation
  if (client.connect(host, port)) {

    // send HTTPS request
    client.print(verb);
    client.print(" ");
    client.print(pathPrefix);
    client.print(lightId);
    client.print(pathPostfix);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(host);
    client.print("\r\n");
    client.print("Content-Length: ");
    client.print(strlen(content));
    client.print("\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(content);

    // read HTTPS response
    while (client.connected() || client.available()) {
      int ch = client.read();
      while (ch >= 0) {
        Serial.print((char) ch);
        ch = client.read();
      }
    }
    Serial.print("\n");
  }
}

void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // keeps watchdog happy
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // built-in LED is inverted
}

void loop() {
  int buttonValue = digitalRead(buttonPin);
  if (buttonState == 0 && buttonValue == HIGH) {
    buttonState = 1; // on
    digitalWrite(ledPin, LOW); // built-in LED is inverted
    sendHueStateWebRequest(2, "{\"on\": true}");
  } else if (buttonState == 1 && buttonValue == LOW) {
    buttonState = 2;
  } else if (buttonState == 2 && buttonValue == HIGH) {
    buttonState = 3; // off
    digitalWrite(ledPin, HIGH); // built-in LED is inverted
    sendHueStateWebRequest(2, "{\"on\": false}");
  } else if (buttonState == 3 && buttonValue == LOW) {
    buttonState = 0;
  }
}

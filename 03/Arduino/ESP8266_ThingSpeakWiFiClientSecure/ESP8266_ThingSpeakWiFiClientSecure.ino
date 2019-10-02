#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

const char *verb = "POST";
const char *path = "/update?api_key=WRITE_API_KEY"; // TODO
const char *host = "api.thingspeak.com";
const int port = 443;

long t0 = 0;

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
}

void loop() {
  long t = millis();
  if (t - t0 > 30 * 1000) {
    t0 = t;

    // read sensor values
    float temp = 23; // TODO
    float humi = 42; // TODO

    // connect to remote host
    BearSSL::WiFiClientSecure client; // use TLS
    client.setInsecure(); // no cert validation
    if (client.connect(host, port)) {
  
      // send HTTPS request
      client.print(verb);
      client.print(" ");
      client.print(path);
      client.print("&field1=");
      client.print(temp);
      client.print("&field2=");
      client.print(humi);
      client.print(" HTTP/1.1\r\n");
      client.print("Host: ");
      client.print(host);
      client.print("\r\n");
      client.print("Connection: close\r\n\r\n");
  
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
}

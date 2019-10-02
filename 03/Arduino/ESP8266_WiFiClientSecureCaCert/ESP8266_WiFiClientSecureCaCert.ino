#include <ESP8266WiFi.h>
#include <time.h>

// based on https://github.com/SensorsIot/HTTPS-for-Makers licensed under
// MIT https://github.com/SensorsIot/HTTPS-for-Makers/blob/master/LICENSE
// CA certificate converted with CertToESP8266.py in the same repository
extern const unsigned char caCert[] PROGMEM;
extern const unsigned int caCertLen;

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

const char *host = "www.howsmyssl.com";
const char *path = "/a/check";
const int port = 443;

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

  Serial.println("Setting system time");
  // call built-in, ESP8266-specific function in time.cpp
  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/time.cpp#L61
  const int timezone = 0;
  const int dst_off = 0;
  configTime(timezone * 3600, dst_off, "pool.ntp.org", "time.nist.gov");
  // wait for time() being adjusted, as a side effect of configTime
  while (time(NULL) < 28800 * 2) {
    delay(500);
  }

  BearSSL::WiFiClientSecure client; // use TLS
  Serial.println("Setting CA certificate ");
  if (!client.setCACert_P(caCert, caCertLen)) {
    Serial.println("Setting CA certificate failed");
  }
  Serial.print("Connecting to host ");
  Serial.println(host);
  if (client.connect(host, port)) {
    Serial.println("Verifying certificate chain ");
    if (client.verifyCertChain(host)) {
      Serial.println("Sending HTTP request");
      client.print("GET ");
      client.print(path);
      client.print(" HTTP/1.1\r\n");
      client.print("Host: ");
      client.print(host);
      client.print("\r\n");
      client.print("Connection: close\r\n\r\n");
  
      Serial.println("Reading HTTP response\n");
      while (client.connected() || client.available()) {
        int ch = client.read();
        while (ch >= 0) {
          Serial.print((char) ch);
          ch = client.read();
        }
      }
    } else {
      Serial.println("Certificate mismatch");
    }
  } else {
    Serial.println("Cannot connect");
  }
}

void loop() {}

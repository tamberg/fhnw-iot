#include <ESP8266WiFi.h>
#include <time.h>
#include "certs.h"

// CA certificate converted with 
// $ pip3 install cryptography
// $ cd ~/Library/Arduino15/packages/esp8266/hardware/esp8266/3.0.2/tools 
// $ python3 cert.py -s www.howsmyssl.com -n howsmyssl

X509List cert(cert_ISRG_Root_X1);

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
    delay(500);
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
  //client.allowSelfSignedCerts();
  Serial.println("Setting CA certificate ");
  client.setTrustAnchors(&cert);
  Serial.print("Connecting to host ");
  Serial.println(host);
  if (client.connect(host, port)) {
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
    Serial.println("Cannot connect, certificate mismatch?");
  }
}

void loop() {}

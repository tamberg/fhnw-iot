// To get the SHA-1 fingerprint, check the certificate in your browser or use the following command on Linux or MacOS:
// openssl s_client -connect www.howsmyssl.com:443 < /dev/null 2>/dev/null | openssl x509 -fingerprint -noout -in /dev/stdin

#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

const char *host = "www.howsmyssl.com"; // via @spiessa
const char *fingerprint = // SHA-1, not very secure anymore
  "8E 07 D1 98 9E 39 40 47 5B 3E 38 E2 98 CF A2 DA 67 76 5E 98";
const char *path = "/a/check";
const int port = 443;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to host ");
  Serial.println(host);
  BearSSL::WiFiClientSecure client; // use TLS
  //client.allowSelfSignedCerts();
  client.setFingerprint(fingerprint);
  if (client.connect(host, port)) {

    Serial.print("Verifying host fingerprint ");
    Serial.println(fingerprint);
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
    Serial.println("Cannot connect or certificate mismatch");
  }
}

void loop() {}

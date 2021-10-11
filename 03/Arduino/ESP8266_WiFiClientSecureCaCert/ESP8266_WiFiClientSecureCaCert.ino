#include <ESP8266WiFi.h>
#include <time.h>

// Chrome > Address bar lock icon > CA Certificate > Drag & Drop Cert
// $ openssl x509 -inform der -in www.howsmyssl.com.cer -out cert.pem 
const char cert_pem [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIEjzCCA3egAwIBAgISBJ1bFpXEv5L7HobG8ytsWpA1MA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMTA4MjIwMTA5MThaFw0yMTExMjAwMTA5MTdaMBwxGjAYBgNVBAMT
EXd3dy5ob3dzbXlzc2wuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEqEor
1NCp4NXR9VBQETs6w5w40yUJlE1LjgVACYWpblrDJjSFcZgU0kb6RKkcDf5fnxhA
nwlSB+aoRcfQKsNqRKOCAn4wggJ6MA4GA1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAU
BggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUZFJk
5sV/CGGC1Qdv+ObbCnfLiJgwHwYDVR0jBBgwFoAUFC6zF7dYVsuuUAlA5h+vnYsU
wsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8vcjMuby5sZW5j
ci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9yZy8wTQYDVR0R
BEYwRIINaG93c215c3NsLmNvbYINaG93c215dGxzLmNvbYIRd3d3Lmhvd3NteXNz
bC5jb22CEXd3dy5ob3dzbXl0bHMuY29tMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcG
CysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5
cHQub3JnMIIBBQYKKwYBBAHWeQIEAgSB9gSB8wDxAHYA9lyUL9F3MCIUVBgIMJRW
juNNExkzv98MLyALzE7xZOMAAAF7a56P3gAABAMARzBFAiBI1ynbicmqTJsTk/Ak
eSNSw6NczE3CGqlHAxU1ghm8MwIhAPQWOiXDGKONhW/h2/On20JakSvIWX7iOd34
Kjp9N832AHcAXNxDkv7mq0VEsV6a1FbmEDf71fpH3KFzlLJe5vbHDsoAAAF7a56Q
aAAABAMASDBGAiEA/ZQa4cM9LdXZ5e9yQrW0JPeXehDgAiUgWLzmSl7ORGUCIQCU
vosfKpHwwzR+BGPtrDLBmZRdwMW1R4C3wT8NjgnlzzANBgkqhkiG9w0BAQsFAAOC
AQEAPlM/1ztbJbRHsP2T9mOMFGNIlfr5wILUufrje7IcnmYYztVenlzsFAtZkBGG
yZQuIhQcNH17KNlsIGP9kPqRXmhuLN3ORNcwy2PK48XtXKMsIGCAVRVvPqOYVDDM
cyoJeYp4t1eywaYnBNzDjIoGxc6hJZUQ1qjyawQRmKdE2ssGXmk0RvQADefEIXC9
nFVzM5/G/HsgQdBrOvEDrX3EudROAcOAiKmzjwpsEtHcZok/JZycvePcEm8BaK7N
fk728on482l5IckUravt2uwquzt61D3qNE+xDA584OApHUAXGwQwiY585PNKR9Gs
Ym77r9Smv9uc3qoTEcA/nbvkrQ==
-----END CERTIFICATE-----
)CERT";

X509List cert(cert_pem);

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

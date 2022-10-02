#include <ESP8266WiFi.h>
#include <time.h>

// Safari > Address bar lock icon > CA Certificate > Drag & Drop Cert
// $ openssl x509 -inform der -in www.howsmyssl.com.cer -out cert.pem 
const char cert_pem [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIEjTCCA3WgAwIBAgISA3c+eqctHMvm7QdG/7MFgpu2MA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMjA4MjIwMjAzNThaFw0yMjExMjAwMjAzNTdaMBwxGjAYBgNVBAMT
EXd3dy5ob3dzbXlzc2wuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEkh5V
dpRDB90ebdIvwRfPBHQG9clrGoRHzGAh7HDNZUrldQXKmJCrmPVQA70GgNfer38i
xjQh8+grPlaTigOa+aOCAnwwggJ4MA4GA1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAU
BggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUTDyr
lAj4Vi74X9eXY4ItXzcW2MUwHwYDVR0jBBgwFoAUFC6zF7dYVsuuUAlA5h+vnYsU
wsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8vcjMuby5sZW5j
ci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9yZy8wTQYDVR0R
BEYwRIINaG93c215c3NsLmNvbYINaG93c215dGxzLmNvbYIRd3d3Lmhvd3NteXNz
bC5jb22CEXd3dy5ob3dzbXl0bHMuY29tMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcG
CysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5
cHQub3JnMIIBAwYKKwYBBAHWeQIEAgSB9ASB8QDvAHYAKXm+8J45OSHwVnOfY6V3
5b5XfZxgCvj5TV0mXCVdx4QAAAGCw4HIwwAABAMARzBFAiB5uVrV3DeDYbTq/42V
+awgYTcIzijqvnXIxj1QPR/rogIhAILas3wcYRwkpGWTc9a4uE9D4fWkKW3ixmwr
fFGRvBT5AHUAQcjKsd8iRkoQxqE6CUKHXk4xixsD6+tLx2jwkGKWBvYAAAGCw4HK
zAAABAMARjBEAiBsh1ZQyhIE8z42QsGqJU+uG09yhZg2r4LrUDVtwy2aywIgQefy
XFh1QyS9v9XpyerRX9gCzbW3aq3zzMy1E8RWZf4wDQYJKoZIhvcNAQELBQADggEB
AAzEr/bUs7/NQrstRW4ob0+mJSc+ce0HrS6deAS9puIEzX6p74G21wJiW0Ed+sHU
30lpnO3k7puNHbB4YoK36AzWcML1QE6rRy6I2vvKAENnCSYUU2qMZ4XMu+8iseWI
Ued+DWN3MufoEjUTfGIgnYSY3BKnwhUPbE0Sl1rkYm9Ds6fzxPT9MWtYUcEG+hnE
KNgXU4Ob99WpwVbPEqrBp9ZB+C9nsRakP3+vlaw3GdOnIdq5TYkqY4i/Tnu7uaZf
qqhyUyInu7ATCiEQzsj6eH6GO/3mbpNQwCEXz+NnUQKLFxpnpdhAmJ41wvvyysXh
dEgERFhCcmCR2Iiuc2zsmQA=
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

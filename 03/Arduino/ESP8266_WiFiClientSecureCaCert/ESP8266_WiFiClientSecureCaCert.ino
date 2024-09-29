#include <ESP8266WiFi.h>
#include <time.h>

// Safari > Address bar lock icon > CA Certificate > Drag & Drop Cert
// $ openssl x509 -inform der -in www.howsmyssl.com.cer -out cert.pem 
const char cert_pem [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIFIzCCBAugAwIBAgISBJkRpikCRwZ8zb1ATiuMwNl4MA0GCSqGSIb3DQEBCwUA
MDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQwwCgYDVQQD
EwNSMTAwHhcNMjQwODI1MjIyMTM3WhcNMjQxMTIzMjIyMTM2WjAcMRowGAYDVQQD
ExF3d3cuaG93c215c3NsLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC
ggEBAMUkSTtzRA/MMu26Q9jm73zv9yG1mPo5xb9USBzIlE/sxK+SRjWORz3qbvIH
VLIZH99jCsT6lLgCfU5LSqt1YbXVEotaZ+tM9XlZdzq56AdGNJlfrzBHAFIw6KTs
poNakVPF7oo99gfuVlUfwRtZhhL8Hqv1thXTcDX1pr73dQ4AwUBzJNemAyIA1NFq
Ka8F1yS73vwb+5ILJRna4kq+Wg3oGF/bbAYkDFZmmjHr+W/YF2pypZdvZRiH10Kt
qg6gbWTMC5R1j2geD05OpVGWHtUqD6rIQMtMvU9J7aB7i/Oide4rmd9Chx7PGCU+
7M4z5gSSKFEM91QLx1t3HFk0x0cCAwEAAaOCAkYwggJCMA4GA1UdDwEB/wQEAwIF
oDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAd
BgNVHQ4EFgQUhsgivrbf27pZEZZ8mVWQqPNsDdowHwYDVR0jBBgwFoAUu7zDR6Xk
vKnGw6RyDBCNojXhyOgwVwYIKwYBBQUHAQEESzBJMCIGCCsGAQUFBzABhhZodHRw
Oi8vcjEwLm8ubGVuY3Iub3JnMCMGCCsGAQUFBzAChhdodHRwOi8vcjEwLmkubGVu
Y3Iub3JnLzBNBgNVHREERjBEgg1ob3dzbXlzc2wuY29tgg1ob3dzbXl0bHMuY29t
ghF3d3cuaG93c215c3NsLmNvbYIRd3d3Lmhvd3NteXRscy5jb20wEwYDVR0gBAww
CjAIBgZngQwBAgEwggEEBgorBgEEAdZ5AgQCBIH1BIHyAPAAdgBIsONr2qZHNA/l
agL6nTDrHFIBy1bdLIHZu7+rOdiEcwAAAZGL1vxdAAAEAwBHMEUCIQDFK+hRU+5i
fB6L6LIwQ9QEerIZWJHKE5qnJodx0YuFbQIgD9Kscpvm5k957/URpA0MbOxP+YQK
LsIgt1CnACIN+AIAdgDuzdBk1dsazsVct520zROiModGfLzs3sNRSFlGcR+1mwAA
AZGL1vxxAAAEAwBHMEUCIQCMKKTYkcnXkB46di3YXuDvihYji/Q0+CpIIJVbA1kJ
VwIgAxgB+mukZoduSpOAfQdeyeReM6V6E0uglgBUaV8jrGwwDQYJKoZIhvcNAQEL
BQADggEBAC3Y2gZNa1EsbqaoI+/QEcT0jTln4NhHDQgQe7Tl5IVJpCvV/cbYP+ha
HpA25RwELfhNkU2ivjhgYhm9WCrLFPjKs21R7NMb9vFFtYBgrZKIjstrLPJmZLOv
vM+FY8qf1n8NEuevbYW3PT2LrOsbR7X6hj5bnzfR3nRI9lv7fxZU07mv9UViw13z
oT3F7rRqa5yLdrDLtGjQjoyIKL+rFbc8J56wmBdkxLVDtCbrVKce1JnEBGQEH1TU
Bo+juYHymEfkXLuxf8MO1kjCuTvvTGxrB6WeOXKQEMJasEuqRK88EkaDV03YBWhw
cSyy1PWEeeniNrUGijqM5lCJ4s/BhTg=
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

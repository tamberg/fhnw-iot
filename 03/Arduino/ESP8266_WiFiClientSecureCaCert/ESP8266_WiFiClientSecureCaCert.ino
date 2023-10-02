#include <ESP8266WiFi.h>
#include <time.h>

// Safari > Address bar lock icon > CA Certificate > Drag & Drop Cert
// $ openssl x509 -inform der -in www.howsmyssl.com.cer -out cert.pem 
const char cert_pem [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIFITCCBAmgAwIBAgISBCHZfCy/RZ03kwM1FAwigxasMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMzA5MTYwMDQxMTJaFw0yMzEyMTUwMDQxMTFaMBwxGjAYBgNVBAMT
EXd3dy5ob3dzbXlzc2wuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKC
AQEA1loQvO+pGESojcJK1HBAj6V20KZALmRSpvum2ZpA2/+g8Bk+VaaHQTRdLLpy
cfznUy8SdCXwzY4lpSXIb82ngSrJY9WayGxsYk/m3HIj0oVEx9PsO69gDhkH5guM
KbxkP+/byrwbHYusaSgj+PZtBveaFZt5smyXPD0nrQxPp3zgsgMlq581obsCqeqH
EjVssT4/MHErkc4EkGGxTpCLAHMMwoqHvgl+7vJi/IALWr/eYacf7LN+3Tmofp3B
AvNhZrXXQl16rSPn0jX1iwRZlRFEN+uEHytgHW4tInZNPXZox4muIx84mrG0NYg7
3/90R9naRM3PVgK5Ql6/K34WUwIDAQABo4ICRTCCAkEwDgYDVR0PAQH/BAQDAgWg
MB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMBAf8EAjAAMB0G
A1UdDgQWBBQu3Qo6xwte97nmq8i4WsYsvLx+eTAfBgNVHSMEGDAWgBQULrMXt1hW
y65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUHMAGGFWh0dHA6
Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3IzLmkubGVuY3Iu
b3JnLzBNBgNVHREERjBEgg1ob3dzbXlzc2wuY29tgg1ob3dzbXl0bHMuY29tghF3
d3cuaG93c215c3NsLmNvbYIRd3d3Lmhvd3NteXRscy5jb20wEwYDVR0gBAwwCjAI
BgZngQwBAgEwggEFBgorBgEEAdZ5AgQCBIH2BIHzAPEAdwC3Pvsk35xNunXyOcW6
WPRsXfxCz3qfNcSeHQmBJe20mQAAAYqbpikTAAAEAwBIMEYCIQDIchaNv6BXbg22
Bwnb5lBgAYRMIb2K4REn6s1YjC3LfAIhANdElG12SvJHOlPDTciLcEesjuOjPE8e
T9z1ivVwokNKAHYArfe++nz/EMiLnT2cHj4YarRnKV3PsQwkyoWGNOvcgooAAAGK
m6YqbQAABAMARzBFAiAHIQPcr0EbhWoy2toWDL+C+KCwHYLeKeFYEpA7fT2hrgIh
AJvDgvc9oAEkZOx+n0Ee4a6A0U7GyiCwoy0bbPs996dCMA0GCSqGSIb3DQEBCwUA
A4IBAQBzytcHgwk/Dy5dtxYhpNd4OLkpZeEMznNWtD4M7z0QqYWcKwW2TKHnVvvW
sMRVFJ/MsZnFcC1riELvh0qefKbre3Mx45MMB+pzNW3VKPwrAbMEYNIyJRMe5YrQ
kvzMRPZRAcI/OY4WweCATeY8pdD8pikOmvLkmDqlzxVnu3UlRFRKx69HO4exKUbO
eIZ3QUs+gbaMBOU9yVNVoGhaLZuVWpEqxeWJX+s9LRYT0e61UX+yVKbD+wdsc2BB
vPXNYut1gA9Pe00SvEq8BmURYshHoo8Tp3KQZcb6xy+qhvpLhvTkQ1U+35DWodPm
KYQ4g3a10bihxTlNw9y3oMDE4RX5
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

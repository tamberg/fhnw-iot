#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO
const int port = 443;

BearSSL::ESP8266WebServerSecure server(port);

// TODO: create your own self signed certificate
// $ openssl req -x509 -nodes -newkey rsa:2048 -keyout key.pem -out cert.pem -days 4096
static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIC5jCCAc4CCQDSIyU2vNsoyTANBgkqhkiG9w0BAQsFADA1MQswCQYDVQQGEwJD
SDESMBAGA1UECAwJWsODwrxyaWNoMRIwEAYDVQQHDAlaw4PCvHJpY2gwHhcNMTkw
MzExMDczNjM0WhcNMzAwNTI4MDczNjM0WjA1MQswCQYDVQQGEwJDSDESMBAGA1UE
CAwJWsODwrxyaWNoMRIwEAYDVQQHDAlaw4PCvHJpY2gwggEiMA0GCSqGSIb3DQEB
AQUAA4IBDwAwggEKAoIBAQDOoPVZDFKRwxzT2XZ4WtlNxMLllf2edcdKNDDSdkjk
ffenhamVJGv87wWPm+SmQKgq4oQTkTFivppn2cwtwZRBJfu2GT4WWqUXwTQ9nkMX
zFK9TMXefXdlgUD3/HcEictf+IoA98q3NQAdkg4CiGpeoFFmwGi746daPUk5NAOj
WCtOyAE6XlKrHINiEBjU0M3LdvDM/ujwNJzi+nTYlJkFHhuPZSCaSWc8srjEhqUa
Gh015OUTwEILWxUnPLvIV0xFQ5kjazUnLxQYDSL542iWz3FrEeG81lNfsGUIZVmP
O5VTZUPGJLtUiVrhuXaoUXZSXNEvgNEfihb9Pso/r9VnAgMBAAEwDQYJKoZIhvcN
AQELBQADggEBADyYcZZwNuMR7QhLucaGNTQjMu0F1NGDpb+5OfRaJp4SXNb0jCA2
vk8ZNaj8k9qavoQIqnfYhvLG4Sc+MdZBctZA+BJXR5YvKceSAyz1CEaHKe/7HBGs
5VXDN1o3NnueaMiUTx1WwtdnqsB/f9vhluqNYimAzrz1DoZMBgGmxKd85PFCOI8e
PS+XNEh6ln3Sc2UFSjwRt0oaQCIXjX34cl5GB1DL+3EonhPtUtTQenHJKeziufMw
Vq120f0aXpnsd2TR+aYRwEZqr+J/eX4TOqE7v4kiRUXhPZ3i2jtJQwp2gfTk43pk
faeHIG4AgctUW7gX56Ddc+ChA1nSszVlvyg=
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQDOoPVZDFKRwxzT
2XZ4WtlNxMLllf2edcdKNDDSdkjkffenhamVJGv87wWPm+SmQKgq4oQTkTFivppn
2cwtwZRBJfu2GT4WWqUXwTQ9nkMXzFK9TMXefXdlgUD3/HcEictf+IoA98q3NQAd
kg4CiGpeoFFmwGi746daPUk5NAOjWCtOyAE6XlKrHINiEBjU0M3LdvDM/ujwNJzi
+nTYlJkFHhuPZSCaSWc8srjEhqUaGh015OUTwEILWxUnPLvIV0xFQ5kjazUnLxQY
DSL542iWz3FrEeG81lNfsGUIZVmPO5VTZUPGJLtUiVrhuXaoUXZSXNEvgNEfihb9
Pso/r9VnAgMBAAECggEBALDX/WQbMcfi75c49TYIuxbeIdgQySGHt+sARBC8f6Jf
VUghE4S7yViO0XmZeNX5XHRnrZZ5m4ThkJD1TNSUauhfM9EkRYkoR0fPaYCbJN0H
WID/ZhIH29oaB5q2jr5pmJAozSQK74phaBnm2Oq+Chu2pjSE1J6KNr762OKGa96t
m4YIDk5fITqjKi2/+wT/OQ1M5R24X4RN8mQPH8eoGFK0rX46bBbnpe6ofELzdPrA
A0jrIeSZC115YcLwbmIC4PlVcmtyEpWLcvECub5ftRIdQyA1JkRLkMYB8y8cpPtt
mtX7JaxwVvoZPuFfaQBnADJPfaLAaRBtg+YlCGy2D+ECgYEA65wW+IpJwMTgyN2J
+PuF8hcFWs/YD2gJOE4lK4N59q4SrSi8/FSKtmI29/+4DKPvPeYW9OkX+sUdwfIu
cHzvuQyPHmgjlzRLSborb+hWq/D4wqb4u3URLmS95oze/3vxTSic7YwBvYKVlMng
LyXsM3IzlKXqwHREqZ47ZIZrQmkCgYEA4ILMQK2b8655/1JfeN8ddqmeLy82i2jo
bONUIenSLw52PUAuj9nn2uosQF16K1VK1szNzoDLttYyX+vdesutX7N30uDeDKOQ
0nrGHFRs5CVy+p1SCMMoFHp7o7+A0drnFa5Wi+Jsgzeqpw3z1N6LahDqiQMlYzrK
gW1lIVh6v08CgYBXaQ90ogl+xQwIOXjIsLge+r9304h7HsPc6dEMdbi39hjjuSt0
CgF0bLesBmlH90H5GvYFYr0MwjiqDamA/jkJ1Ej8pPIfVL1h2vnmiLIvFDWWMdAx
69q19ZYmhLju3BLH53tg/gJfRqUsEEFl/JHG/GV6jpurTvdyXkv2nZAJkQKBgQCh
ERVJ/YMZon3Bj64QwDiaDfv/K+b8wwmkPL76CY6R1dU8AhSlfRYads/CaY2hEvK7
AXlRAhOkGy1OpZ9iXkcGlnGLZ2nGNX5LkmsYnfT1BGzloQHeaWwSrQCQrG3VoC8S
3d0XRBzHUqNqb9cTS4lHUdWA8DHda6smiX35G0oMfwKBgQC0ctftqXs3nh4srrjn
GMQRWAsPuqkHSa8EnOu0fpW4Bn9Z1lkwim/OykqMBcjoIFQXILm+TwhjrZG8E2r2
Ms8wv7RBjV3fHmLZTRd1aWFtk1ho0aZLBuktvZb7Cscgejj0VtjYqZgw+0Ct06zW
g48lWDY9mlD7rHhW4zDGQA6C1A==
-----END PRIVATE KEY-----
)EOF";

void handleRoot() {
  server.send(200, "text/plain", "It works!");
}

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());

  server.setRSACert(
    new BearSSL::X509List(serverCert), 
    new BearSSL::PrivateKey(serverKey));
  server.on("/", handleRoot);
  server.begin();
  Serial.print("Listening on port ");
  Serial.println(port);
}

void loop() {
  server.handleClient();
}

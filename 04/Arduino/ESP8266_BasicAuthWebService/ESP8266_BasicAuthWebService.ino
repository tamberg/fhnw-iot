#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO
// https://ddg.co/?q=base64+encode+MY_USER%3AMY_PASSWORD
const char *storedCreds = "MY_BASE64_ENCODED_CREDENTIALS"; // TODO
const int maxCredsLen = 64;
const int port = 80;

WiFiServer server(port);

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());
  server.begin();
}

void readStringToEndOfLine(WiFiClient client, char *s, int maxLen) {
  memset(s, '\0', maxLen); // fill string with '\0'
  int i = 0;
  int ch = client.read();
  while (i < (maxLen - 1) && ch != -1 && ch != '\r' && ch != '\n') {
    s[i] = ch;
    i++;
    ch = client.read();
  }
}

void send200Response(WiFiClient client) {
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Length: 0\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");
}

void send401Response(WiFiClient client) {
  client.print("HTTP/1.1 401 Unauthorized\r\n");
  client.print("WWW-Authenticate: Basic\r\n");
  client.print("Content-Length: 0\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");
}

void loop() {
  WiFiClient client = server.available();
  if (client && client.connected()) {
    Serial.println(client.remoteIP());
    if (client.find("Authorization: Basic ")) {
      char creds[maxCredsLen];
      readStringToEndOfLine(client, creds, maxCredsLen);
      Serial.println(creds);
      client.find("\r\n\r\n"); // Ignore further request headers
      if (strcmp(storedCreds, creds) == 0) {
        send200Response(client);
      } else {
        send401Response(client);
      }
    } else {
      send401Response(client);
    }
    delay(1); // Give Web browser time to receive data
    client.stop();
  }
}

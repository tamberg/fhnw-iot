#include <ESP8266WiFi.h>
#include <time.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // keeps watchdog happy
  }
  Serial.print("Connected to network, local IP = "); 
  Serial.println(WiFi.localIP());

  // call built-in, ESP8266-specific function in time.cpp
  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/time.cpp#L61
  const int timezone = 0;
  const int dst_off = 0;
  configTime(timezone * 3600, dst_off, "pool.ntp.org", "time.nist.gov");
  // wait for time() being adjusted, as a side effect of configTime
  while (time(NULL) < 28800 * 2) {
    delay(500);
  }
}

void loop() {
  // number of seconds elapsed since the Epoch, 
  // 1970-01-01 00:00:00 +0000 (UTC).
  time_t now = time(NULL);

  // http://man7.org/linux/man-pages/man3/ctime.3.html
  // same as asctime(localtime(&now))
  Serial.print(ctime(&now));

  // ISO 8601 UTC, e.g. 2019-03-05T00:00:00Z
  const char *format = "%Y-%m-%dT%H:%M:%SZ";
  const struct tm *t = gmtime(&now);
  const size_t maxLen = 21;
  char s[maxLen];
  size_t len = strftime(s, maxLen, format, t);
  Serial.println(s);
  
  delay(1000);
}

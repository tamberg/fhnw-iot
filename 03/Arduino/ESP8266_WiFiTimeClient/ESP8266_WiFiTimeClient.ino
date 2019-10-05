#include <ESP8266WiFi.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO

time_t getTime() {
  // returns current Unix time, i.e. seconds since Epoch
  return time(NULL);
}

void setTime(time_t t) {
  // expects Unix time, i.e. seconds since Epoch
  struct timeval now = { .tv_sec = t, .tv_usec = 0 };
  struct timezone utc = { .tz_minuteswest = 0, .tz_dsttime = 0 };
  settimeofday(&now, &utc);
}

time_t parseTimeFromDateHeader(String header) {
  // expects HTTP date header in UTC, e.g.
  // "Date: Sat, 02 Mar 2019 13:37:00",
  // returns Unix time, i.e. seconds since Epoch
  time_t t = 0;
  if (header.indexOf("Date: ") == 0) {
    // e.g. "Date: Sat, 02 Mar 2019 13:37:00\r"
    String date = header.substring(6, 6 + 25);
    Serial.print(date);
    Serial.print("\n");
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm)); // initialise all fields to 0
    char *r = strptime(date.c_str(), "%a, %d %b %Y %H:%M:%S", &tm);
    if (r != NULL) { // parsing succeeded
      t = mktime(&tm); // seconds since Epoch
    }
  }
  return t;
}

void printTime(time_t t) {
  // expects Unix time, i.e. seconds since Epoch,
  // prints ISO format, e.g. 2019-03-02T13:37:00Z
  struct tm *tm = gmtime(&t);
  char s[21];
  size_t r = strftime(s, 21, "%Y-%m-%dT%H:%M:%SZ", tm);
  s[r] = '\0'; // make sure string is terminated
  Serial.print(s);
}

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to network ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // keeps watchdog happy
  }
  Serial.print("Connected, IP = "); 
  Serial.println(WiFi.localIP());

  WiFiClient client;
  // http://www.esp8266.com/viewtopic.php?f=19&t=809
  if (client.connect("google.com", 80)) {
    // write HTTP request:
    client.print("HEAD / HTTP/1.1\r\n" \
      "Host: google.com\r\n" \
      "Connection: close\r\n\r\n");
    // read HTTP response:
    client.setTimeout(10000); // ms
    while (client.connected() || client.available()) {
      String  header = client.readStringUntil('\n');
      Serial.print(header);
      Serial.print("\n");
      if (header.indexOf("Date: ") == 0) {
        time_t t = parseTimeFromDateHeader(header);
        setTime(t); // sync system time to server
      }
    }
  }
}

void loop() {
  time_t t = getTime();
  printTime(t);
  Serial.print("\n");
  delay(1000);
}

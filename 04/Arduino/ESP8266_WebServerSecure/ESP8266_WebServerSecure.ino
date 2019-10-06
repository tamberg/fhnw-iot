#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>

const char *ssid = "MY_SSID"; // TODO
const char *password = "MY_PASSWORD"; // TODO
const int port = 443;

const char *basicAuthUsername = "MY_USERNAME"; // TODO
const char *basicAuthPassword = "MY_PASSWORD"; // TODO

BearSSL::ESP8266WebServerSecure server(port);

// TODO: create your own self signed certificate
// $ openssl req -x509 -nodes -newkey rsa:2048 -keyout key.pem -out cert.pem -days 4096
static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIICljCCAX4CCQChm1xpx+PkPTANBgkqhkiG9w0BAQsFADANMQswCQYDVQQGEwJD
SDAeFw0xOTEwMDYxNTM3NTJaFw0zMDEyMjMxNTM3NTJaMA0xCzAJBgNVBAYTAkNI
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA82MSLMYHkSOSDkqAWKV8
tiV+pxKG1LT5OK8UeCDVJpo9bK6n8hba0wwZ9BAe9znvtu8zy4bXY1QL+/vdN62r
1qyzUAuERO8E8rr9OKzRdaq+EWiW8kpmi4YDWG99XRArXHik9K5+Crh9UMu9fCUg
rGQQIe+0s2dRHIh/JFTllDPn7134o5TTu9ec1jkPJcKUaj0lqiIO1RXsgyqjPmFv
PRd8fRSCuw4vWVk8t+swb5xfNNGZw2J9N00rcw7hlSRgeK6wVRuACwL5i9UVI4tQ
W1SGom40gePzuCxlyMf3FAZ2qiYylm8wtfYodGdQn+VIqGcjN6X1xiJNz3aNGoIs
6wIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQCa/gZ9PjWtptiMntNWBfBfaIgt6RYP
Q2WDXRCFoztds3I3s3joMCFxroAymXcF4BnRI9H8m7gav6dHAUMoAN2vXY8Otwce
1DflplPocB/kCww+2M4K4GIDQN71Tc0lwKTGCmuBUazWAnP8YBwrnPpy7dIWq4qF
CceeHNIuB6sxsWKbXs1En7mITcFsMCH9ylSIawLcL9XhctbkDFB1TFljm/Sq0AM6
nt0v51rA47LVvdMA/R/qOYDgMrw0lyrkjnGjGVKP/KeKQi8kcPYk5r2gJv9RyMIV
VQGCKkB7DdtM7sQ0KwbN0V0sODsKWFBLCJXZ5Qe+eDl3CtmGCNxDCJ8B
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDzYxIsxgeRI5IO
SoBYpXy2JX6nEobUtPk4rxR4INUmmj1srqfyFtrTDBn0EB73Oe+27zPLhtdjVAv7
+903ravWrLNQC4RE7wTyuv04rNF1qr4RaJbySmaLhgNYb31dECtceKT0rn4KuH1Q
y718JSCsZBAh77SzZ1EciH8kVOWUM+fvXfijlNO715zWOQ8lwpRqPSWqIg7VFeyD
KqM+YW89F3x9FIK7Di9ZWTy36zBvnF800ZnDYn03TStzDuGVJGB4rrBVG4ALAvmL
1RUji1BbVIaibjSB4/O4LGXIx/cUBnaqJjKWbzC19ih0Z1Cf5UioZyM3pfXGIk3P
do0agizrAgMBAAECggEAXA5YzX6JVigyEh2ztdPB+9LquJ6VSNa6fOobv+QLOvKT
0Quuki56zeCB4IJIdBIBzRtkZ9K0nOkpL0HwRiL2H9hqahgxCzGsmXKIXGthPjQ6
d7MrhLEHYDZsBYMF07FN9UfcfH+TRNLK80hcfxw743hlcbCMlF67myM1k7RYsVjD
wBw/8xCaVUEpOSH0nZgkk/ziKqOk/6H1pIF+K73EbTI/nRk3GlxdkvfRcN2y1Aq0
Oc56gHhTtxRUGU0Z44jHDLEuCKiIMx5KA3NzETJgQCiuZxp0XPR6P4uD6NRQXifE
fDt09sJ+m3d7UWM39rwUstGyvvMq4SqojXvuDldFoQKBgQD+qLBvx+BmJ8e49mIt
xIsRC/wg2UegA91XP+ViAIXxr6CYXB1RH08q9F6/zqw9QuLhLZbkPjTh24+RIqGt
M31kNrY8LYfUZfvhSvuFD3O5ROrdKT+wcjMc+FBFk4vu9dvmlM6FDaOVqJE90vm7
e0Kh4l6n0frtDOIASBWmS0GMbQKBgQD0qy+ULy/ucoX4gX6/WD6jLP87BReTbPi+
1jhhM/wftM42d4sfubOdvsXQJPK8S/B5OyDjDhe14r4GPb3u7qh8t7PZt7kr4B34
DftUIWukR9n/5ZI0Agy4ok4GIziMnaYDF14G/VZaVxH6XF9RCyV8sJF9/jUTAab7
hsD+f40XtwKBgQCbBmYiJjNc/OqAlry9z+A4yGr8NEl+B9w+uQ8S0e64aooBNlJC
iCTRqWHkshP4Vzd2jSJGX/ghZl/HoqV5ota6ovPdPnppuovTYqorHnT8XA/+ApZa
+6pSAHgdYuzU06bCf+QY62yiNrP0RvLwRuERW5Tz5Mg0dps+QKOtUDLfIQKBgQCQ
5nUmIJAVerPHCgIjlkJ6ZyK9NHXS9N2YP8xiHOR7bpwV+9RkNJmgmaQTcl5jj+Ha
Gl+OuQDn0mJR1e0PfT5BppYYIcvDpVZ6WuV23NggEgGUoMRmkrQwAMeOsPtDo/X2
rLceEpk6pYhbKqu0lpJb90IjSRLrr7XSDutrjeTknwKBgAQurcjqJEd3iJhHpnv/
tDyQ9kaIxrlHpP/REQISwYcaD4JijU82Wg99rpiGZ4vT2WhV25Dg1ubEEuesmzA6
8TuxNJrdD0PD9Vcb4KKKrkNj4IvDthippreZJ6rvSEzGGJBnxiSRApCpi8BXPsVs
znybL/IBQctH68BjC4r940b2
-----END PRIVATE KEY-----
)EOF";

void handleRoot() {
  if (server.authenticate(basicAuthUsername, basicAuthPassword)) {
    server.send(200, "text/plain", "It works!");
  } else {
    // sends "401 Unauthorized", with a
    // "WWW-Authenticate: Basic" header
    server.requestAuthentication();
  }
}

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

  server.setRSACert(
    new BearSSL::X509List(serverCert), 
    new BearSSL::PrivateKey(serverKey));

  // add one handler per path
  server.on("/", handleRoot);
  
  server.begin();
  Serial.print("Listening on port ");
  Serial.println(port);
}

void loop() {
  server.handleClient();
}

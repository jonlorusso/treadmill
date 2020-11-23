#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

#define TREADMILL_PIN 15
#define INCREMENT 1

#define MAX_SPEED 75
#define STOP      0

int currentSpeed = STOP;
int newSpeed = STOP;


const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void setup(void) {
  pinMode(TREADMILL_PIN, OUTPUT);
  analogWrite(TREADMILL_PIN, min(max(currentSpeed, 0), STOP));
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/speed", []() {
    if (validSpeed(server.arg(0).toInt())) {
      newSpeed = i;
    }
      
    server.send(200, "text/plain", "Speed: " + input);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  if (currentSpeed < newSpeed) {
    currentSpeed = min(MAX_SPEED, currentSpeed + INCREMENT);
    analogWrite(TREADMILL_PIN, currentSpeed);
    delay(10);
  } else if (currentSpeed > newSpeed) {
    currentSpeed = max(STOP, currentSpeed - INCREMENT);
    analogWrite(TREADMILL_PIN, currentSpeed);
    delay(10);
  }
}

boolean validSpeed(int speed) {
    return speed >= STOP && speed <= MAX_SPEED;
}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PIN_LED 5
#define PIN_PWMA 0
#define PIN_AIN2 4
#define PIN_AIN1 13
#define PIN_STBY 12

const char* ssid = "XXX";
const char* password = "XXX";

ESP8266WebServer server(80);

void drive(int speed, int dir) {
  digitalWrite(PIN_STBY, HIGH);
  if (dir > 0) {
    digitalWrite(PIN_AIN1, HIGH);
    digitalWrite(PIN_AIN2, LOW);
    analogWrite(PIN_PWMA, speed);
  } else if (dir < 0) {
    digitalWrite(PIN_AIN1, LOW);
    digitalWrite(PIN_AIN2, HIGH);
    analogWrite(PIN_PWMA, speed);
  }
}

void brake() {
  digitalWrite(PIN_AIN1, HIGH);
  digitalWrite(PIN_AIN2, HIGH);
  analogWrite(PIN_PWMA, 0);
}

void standby() {
  digitalWrite(PIN_STBY, LOW);
}

void cycle(int dir) {
  // Drive the motor for 800ms, then brake for 100ms.
  drive(300, dir);
  delay(800);
  brake();
  delay(100);
  standby();
}

void setup() {
  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_STBY, OUTPUT);
  standby();

  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_PWMA, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", []() {
    server.send(200, "text/plain", "fishfood\n");
  });

  server.on("/fwd", []() {
    server.send(200, "text/plain", "OK\n");
    cycle(+1);
  });

  server.on("/rev", []() {
    server.send(200, "text/plain", "OK\n");
    cycle(-1);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

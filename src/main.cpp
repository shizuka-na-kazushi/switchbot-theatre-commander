#include <Arduino.h>
#include <WiFi.h>
#include "ServoWebServer.h"
#include <ESP32Servo.h>
#include <ESPmDNS.h>
#include "TimeClient.h"

Servo servo1;

const int SERVO_PIN = (1);

const int minUs = 500;
const int maxUs = 2400;

// Network credentials
const char* ssid     = "TP-Link_F5AC";
const char* password = "56056891";

const char* mdnsHostname = "volume";  // can call to "volume.local"

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("the program has been initialized!");

  // setup servo
  servo1.setPeriodHertz(50);
  servo1.attach(SERVO_PIN, minUs, maxUs);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // pinMode(SERVO_PIN, OUTPUT);
  theServer.attachServo(&servo1);
  theServer.setup();

  // mdns
  MDNS.begin(mdnsHostname);

  // time client (NTP)
  timeClient.begin();
}


void loop() {

  // delay(2000);
  // servo1.write(0);
  // Serial.println("servo was set to 0");

  // delay(2000);
  // servo1.write(15);
  // Serial.println("servo was set to 90");
  
  // delay(2000);
  // servo1.write(30);
  // Serial.println("servo was set to 180");
  

  //  delay(1000);
  //  digitalWrite(SERVO_PIN, HIGH);
  //  delay(1000);
  //  digitalWrite(SERVO_PIN, LOW);

  theServer.loop(); 
  timeClient.update();
}

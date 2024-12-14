#ifndef __SERVO_WEB_SERVER_H__
#define __SERVO_WEB_SERVER_H__

#include <ESP32Servo.h>

class HeaderInfo {
public:
  String  command;
  String  path;
  String  queryStrings;
};

class ServoWebServer {
protected:
    // Current time
    unsigned long currentTime = millis();
    // Previous time
    unsigned long previousTime = 0; 
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 2000;

    // the Servo
    Servo*  m_pServo;
public:
  ServoWebServer();
  virtual ~ServoWebServer();

  void attachServo(Servo* pServo) {m_pServo = pServo;};
  void setup();
  void loop();

protected:
  boolean processHttpHeader(const String& header, HeaderInfo& headerInfo);
  boolean dispatch(const HeaderInfo& header, WiFiClient& client);
};

extern ServoWebServer theServer;

#endif // !__SERVO_WEB_SERVER_H__

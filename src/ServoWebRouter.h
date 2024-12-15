#ifndef __SERVO_WEB_ROUTER_H__
#define __SERVO_WEB_ROUTER_H__

#include <Arduino.h>
#include <WiFi.h>
#include "ServoWebServer.h"

#define WEB_ROUTE_MACRO_BEGIN() if (0) {}

#define WEB_ROUTE_EXECUTE_1(X_PATH, X_HANDLER, X_PARAM_1)  \
    else if (header.path == X_PATH) {   \
    return X_HANDLER(header, client, X_PARAM_1);\
  }

#define WEB_ROUTE_EXECUTE_2(X_PATH, X_HANDLER, X_PARAM_1, X_PARAM_2)  \
    else if (header.path == X_PATH) {   \
    return X_HANDLER(header, client, X_PARAM_1, X_PARAM_2);\
  }

#define WEB_ROUTE_MACRO_END() 

/** Handlers here */
boolean respondWithChangeLevel(const HeaderInfo& header, WiFiClient& client, int level, Servo* pServo);
boolean respondExecuteSwitchbotScene(const HeaderInfo& header, WiFiClient& client, const String& sceneId);
boolean respondExecuteSwitchbotSceneWithRedirect(const HeaderInfo& header, WiFiClient& client, const String& sceneId);

#endif // !__SERVO_WEB_ROUTER_H__


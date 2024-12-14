
#include "WiFi.h"
#include "ServoWebServer.h"
#include <pgmspace.h>
#include "BinFiles.h"
#include "CallSwitchbot.h"
#include "SceneSecret.h"

ServoWebServer theServer;
// Web server on port 80 (http)
WiFiServer server(80);

static int LevelToServoDegs[] = {0, 20, 40};

ServoWebServer::ServoWebServer() : m_pServo(nullptr) 
{

}

ServoWebServer::~ServoWebServer() 
{

}

void ServoWebServer::setup()
{
  // Current time
  currentTime = millis();
  // Previous time
  previousTime = 0; 

  // WiFiServer::begin()
  server.begin();
}

boolean ServoWebServer::processHttpHeader(const String& header, HeaderInfo& headerInfo) 
{
     // ヘッダーが空の場合は処理しない
    if (header.length() == 0) {
        return false;
    }

    // スペースで区切られた最初の部分を抽出（メソッド）
    int firstSpaceIndex = header.indexOf(' ');
    if (firstSpaceIndex == -1) {
        return false;
    }
    headerInfo.command = header.substring(0, firstSpaceIndex);

    // 2番目のスペースまでの部分を抽出（パス）
    int secondSpaceIndex = header.indexOf(' ', firstSpaceIndex + 1);
    if (secondSpaceIndex == -1) {
        return false;
    }
    String pathAndQuery = header.substring(firstSpaceIndex + 1, secondSpaceIndex);

    // パスとクエリ文字列を分割
    int queryIndex = pathAndQuery.indexOf('?');
    if (queryIndex == -1) {
        // クエリ文字列がない場合
        headerInfo.path = pathAndQuery;
        headerInfo.queryStrings = "";
    } else {
        // クエリ文字列がある場合
        headerInfo.path = pathAndQuery.substring(0, queryIndex);
        headerInfo.queryStrings = pathAndQuery.substring(queryIndex + 1);
    }

    return true;
}

static boolean respondGenericError(const HeaderInfo& header, WiFiClient& client) 
{
    client.println("HTTP/1.1 500 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html lang=\"ja\">");
    client.println("<head><title>not found</title></head><body>Internal server error</body>");
    client.println("</html>");
    return true;
}

static boolean respondNoResource(const HeaderInfo& header, WiFiClient& client) 
{
    client.println("HTTP/1.1 404 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html lang=\"ja\">");
    client.println("<head><title>not found</title></head><body>Error: 404 Not Found</body>");
    client.println("</html>");
    return true;
}

static boolean respondMainHtml(const HeaderInfo& header, WiFiClient& client) 
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    // client.println("<html lang=\"ja\">");
    // client.println("<head><title>Servo switcher</title></head><body>");

    // client.println("<li><a href='/level0'>Level 0</a></li>");
    // client.println("<li><a href='/level1'>Level 1</a></li>");
    // client.println("<li><a href='/level2'>Level 2</a></li>");

    // client.println("</body>");
    // client.println("</html>");
client.println("<!DOCTYPE html>");
client.println("<html lang=\"ja\">");
client.println("<head>");
client.println("    <meta charset=\"UTF-8\">");
client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
client.println("    <title>Local theater system</title>");
client.println("    <style>");
client.println("        body, html {");
client.println("            margin: 0;");
client.println("            padding: 0;");
client.println("            height: 100%;");
client.println("            justify-content: center;");
client.println("            align-items: center;");
client.println("            font-family: Arial, sans-serif;");
client.println("            background-color: #f0f0f0;");
client.println("        }");
client.println("");
client.println("        .container {");
client.println("            width: 100%;");
client.println("            padding: 50px 20px;");
client.println("            box-sizing: border-box;");
client.println("            display: flex;");
client.println("            flex-direction: column;");
client.println("            gap: 20px;");
client.println("            height: 90vh;");
client.println("        }");
client.println("");
client.println("        .header {");
client.println("            background-color: #ffffff;");
client.println("            border: 2px solid #e0e0e0;");
client.println("            border-radius: 10px;");
client.println("            padding: 20px;");
client.println("            display: flex;");
client.println("            flex-direction: column;");
client.println("            align-items: center;");
client.println("            margin-bottom: 20px;");
client.println("        }");
client.println("");
client.println("        .header-title {");
client.println("            font-size: 24px;");
client.println("            font-weight: bold;");
client.println("            margin-bottom: 15px;");
client.println("            color: #333;");
client.println("        }");
client.println("");
client.println("        .header-buttons {");
client.println("            display: flex;");
client.println("            gap: 20px;");
client.println("            width: 100%;");
client.println("            justify-content: center;");
client.println("        }");
client.println("");
client.println("        .button {");
client.println("            flex: 1;");
client.println("            display: flex;");
client.println("            justify-content: center;");
client.println("            align-items: center;");
client.println("            color: white;");
client.println("            border: none;");
client.println("            border-radius: 10px;");
client.println("            font-size: 18px;");
client.println("            cursor: pointer;");
client.println("            transition: background-color 0.3s ease;");
client.println("            text-decoration: none;");
client.println("            height: 65px;");
client.println("        }");
client.println("");
client.println("        .button {");
client.println("            background-color: #3498db;");
client.println("        }");
client.println("        .button:hover {");
client.println("            background-color: #2980b9;");
client.println("        }");
client.println("");
client.println("        .button-on {");
client.println("          background-image: url(\"/switch-on.png\");");
client.println("          background-repeat: no-repeat;");
client.println("          background-position: center;");
client.println("          background-size: 50px;");
client.println("        }");
client.println("        .button-off {");
client.println("          background-image: url(\"/switch-off.png\");");
client.println("          background-repeat: no-repeat;");
client.println("          background-position: center;");
client.println("          background-size: 50px;");
client.println("        }");
client.println("");
client.println("        /* カラーバリエーション */");
client.println("        .color1 {");
client.println("            background-color: #ff7a7a;");
client.println("        }");
client.println("        .color1:hover {");
client.println("            background-color: #ffbc7a;");
client.println("        }");
client.println("");
client.println("        .color2 {");
client.println("            background-color: #ff6666;");
client.println("        }");
client.println("        .color2:hover {");
client.println("            background-color: #ffb266;");
client.println("        }");
client.println("");
client.println("        .color3 {");
client.println("            background-color: #ff5151;");
client.println("        }");
client.println("        .color3:hover {");
client.println("            background-color: #ffa851;");
client.println("        }");
client.println("");
client.println("        .color4 {");
client.println("            background-color: #efefef;");
client.println("        }");
client.println("        .color4:hover {");
client.println("            background-color: #afafaf;");
client.println("        }");
client.println("");
client.println("    </style>");
client.println("    <script>");
client.println("      function changeVol(index) {");
client.println("        const table = {\"1\": \"/level0\", \"2\": \"/level1\", \"3\": \"/level2\"};");
client.println("        fetch(table[index]);");
client.println("      }");
client.println("      function execScene(enabled) {");
client.println("        fetch(enabled ? \"/theatreOn\" : \"/theatreOff\");");
client.println("      }");
client.println("    </script>");
client.println("</head>");
client.println("<body>");
client.println("    <div class=\"container\">");
client.println("        <div class=\"header\">");
client.println("            <div class=\"header-title\">Theater</div>");
client.println("            <div class=\"header-buttons\">");
client.println("                <button class=\"button color4 button-on\" onclick=\"execScene(true)\"></button>");
client.println("                <button class=\"button color4 button-off\" onclick=\"execScene(false)\"></button>");
client.println("            </div>");
client.println("        </div>");
client.println("");
client.println("        <button class=\"button color1\" onclick=\"changeVol(1)\">Level 1</button>");
client.println("        <button class=\"button color2\" onclick=\"changeVol(2)\">Level 2</button>");
client.println("        <button class=\"button color3\" onclick=\"changeVol(3)\">Level 3</button>");
client.println("    </div>");
client.println("</body>");
client.println("</html>");

    return true;
}

static boolean respondWithChangeLevel(const HeaderInfo& header, WiFiClient& client, int level, Servo* pServo) 
{
    if (pServo && (0 <= level) && (level < sizeof(LevelToServoDegs) / sizeof(LevelToServoDegs[0]))) {
      Serial.print("\nLevel= ");
      Serial.print(level);
      Serial.print("; deg= ");
      Serial.print(LevelToServoDegs[level]);
      Serial.println("");Serial.println("");

      pServo->write(LevelToServoDegs[level]);
    }

    return respondMainHtml(header, client);
}

static boolean respondFavicon(const HeaderInfo& header, WiFiClient& client) 
{
  // faviconレスポンス
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/x-icon");
  client.println("Connection: close");
  client.println();

  // バイナリデータの書き込み
  for (size_t i = 0; i < sizeof(favicon_data); i++) {
    client.write(pgm_read_byte(&favicon_data[i]));
  }
  return true;
}

static boolean respondBinFile(const HeaderInfo& header, WiFiClient& client, const uint8_t* data, size_t length, const String& contentType) 
{
  // faviconレスポンス
  client.println("HTTP/1.1 200 OK");
  client.print("Content-Type: ");
  client.println(contentType);
  client.println("Connection: close");
  client.println();

  // バイナリデータの書き込み
  for (size_t i = 0; i < length; i++) {
    client.write(pgm_read_byte(&data[i]));
  }
  return true;
}


static boolean respondExecuteSwitchbotScene(const HeaderInfo& header, WiFiClient& client, const String& sceneId) 
{
  CallSwitchbot callSwitchbot;

  // faviconレスポンス
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  if (callSwitchbot.executeScene(sceneId)) {
    client.println("{code: \"success\"}");
    return true;
  } else {
    client.println("{error: \"internal error\"}");
    return true;
  }
}

#define BIN_FILE_SIZEOF(x)  (sizeof(x) / sizeof(x[0]))

boolean ServoWebServer::dispatch(const HeaderInfo& header, WiFiClient& client) 
{
  if (header.command == "GET") {
    if (header.path == "/" || header.path == "index.html") {
      return respondMainHtml(header, client);
    } else if (header.path == "/level0") {
      return respondWithChangeLevel(header, client, 0, m_pServo);
    } else if (header.path == "/level1") {
      return respondWithChangeLevel(header, client, 1, m_pServo);
    } else if (header.path == "/level2") {
      return respondWithChangeLevel(header, client, 2, m_pServo);
    } else if (header.path == "/theatreOn") {
      return respondExecuteSwitchbotScene(header, client, SWITCHBOT_SCENE_ID_THEATRE_ON);
    } else if (header.path == "/theatreOff") {
      return respondExecuteSwitchbotScene(header, client, SWITCHBOT_SCENE_ID_THEATRE_OFF);
    } else if (header.path == "/favicon.ico") {
      return respondBinFile(header, client, favicon_data, BIN_FILE_SIZEOF(favicon_data), "image/x-icon");
    } else if (header.path == "/switch-on.png") {
      return respondBinFile(header, client, switch_on_data, BIN_FILE_SIZEOF(switch_on_data), "image/png");
    } else if (header.path == "/switch-off.png") {
      return respondBinFile(header, client, switch_off_data, BIN_FILE_SIZEOF(switch_off_data), "image/png");
    }
    return respondNoResource(header, client);
  } else {
    return respondGenericError(header, client);
  }
}

void ServoWebServer::loop()
{
  WiFiClient client = server.available();   
   
  // Client Connected
  if (client) {                             
    // Set timer references
    currentTime = millis();
    previousTime = currentTime;
     
    // Print to serial port
    Serial.println("New Client."); 
     
    // String to hold data from client
    String currentLine; 
    // Variable to store the HTTP request
    String header;
     
    // Do while client is connected
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            HeaderInfo headerInfo;
            if (processHttpHeader(header, headerInfo))
            {
              dispatch(headerInfo, client);
            } else {
              respondGenericError(headerInfo, client);
            }
            break;
          } else { 
            // New lline is received, clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }
}
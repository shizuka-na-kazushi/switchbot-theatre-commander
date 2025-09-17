
#include <WiFi.h>
#include <pgmspace.h>
#include "ServoWebServer.h"
#include "ProgmemStream.h"

ServoWebServer theServer;
// Web server on port 80 (http)
WiFiServer server(80);

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


boolean respondGenericError(const HeaderInfo& header, WiFiClient& client) 
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

boolean respondNoResource(const HeaderInfo& header, WiFiClient& client) 
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

boolean respondBinFile(const HeaderInfo& header, WiFiClient& client, const uint8_t* data, size_t length, const String& contentType) 
{
  // faviconレスポンス
  client.println("HTTP/1.1 200 OK");
  client.print("Content-Type: ");
  client.println(contentType);
  client.println("Connection: close");
  client.println();

  ProgmemStream stream(data, length);
  client.write(stream);
  return true;
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
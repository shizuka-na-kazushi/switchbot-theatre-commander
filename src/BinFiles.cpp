
// !!!!!!!!!!!!     DO NOT EDIT THIS FILE DIRECTORY        !!!!!!!!!!!!
//
// This file need to be generated by 'build-contents.js'
// It will generate the code accordance with files in 'src/www'
//
// !!!!!!!!!!!!     DO NOT EDIT THIS FILE DIRECTORY        !!!!!!!!!!!!

#include <WiFi.h>
#include "ServoWebServer.h"
#include "BinFiles.h"

#define WEB_ROUTE_BIN_FILE_DEF(X_PATH, X_VARIABLE, X_CONTENT_TYPE)  \
    else if (header.path == X_PATH) {   \
    return respondBinFile(header, client, X_VARIABLE, sizeof(X_VARIABLE) / sizeof(X_VARIABLE[0]), X_CONTENT_TYPE);\
  }

boolean dispatchBinFiles(const HeaderInfo& header, WiFiClient& client) {
  if (0) {} 

  WEB_ROUTE_BIN_FILE_DEF("/favicon.ico", binFile_favicon_ico, "image/x-icon")
  WEB_ROUTE_BIN_FILE_DEF("/index.html", binFile_index_html, "text/html")
  WEB_ROUTE_BIN_FILE_DEF("/manifest.json", binFile_manifest_json, "application/json")
  WEB_ROUTE_BIN_FILE_DEF("/speaker144.png", binFile_speaker144_png, "image/png")
  WEB_ROUTE_BIN_FILE_DEF("/speaker16.png", binFile_speaker16_png, "image/png")
  WEB_ROUTE_BIN_FILE_DEF("/speaker180.png", binFile_speaker180_png, "image/png")
  WEB_ROUTE_BIN_FILE_DEF("/speaker48.png", binFile_speaker48_png, "image/png")
  WEB_ROUTE_BIN_FILE_DEF("/switch-off.png", binFile_switch_off_png, "image/png")
  WEB_ROUTE_BIN_FILE_DEF("/switch-on.png", binFile_switch_on_png, "image/png")

  return false; 
}
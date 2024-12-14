
#include <WiFi.h>
#include <HTTPClient.h>
#include <mbedtls/md.h>
#include <mbedtls/base64.h>
#include "CallSwitchbot.h"
#include "TimeClient.h"
#include "SwitchBotSecret.h"

// constants
const String BASE_URL = "https://api.switch-bot.com/v1.1";

// Base64エンコード関数
static String base64Encode(const uint8_t* data, size_t length) {
  size_t encodedLength = 0;
  
  // まず必要なバッファサイズを計算
  mbedtls_base64_encode(NULL, 0, &encodedLength, data, length);
  
  // バッファを確保
  uint8_t* encodedBuffer = (uint8_t*)malloc(encodedLength + 1);
  encodedBuffer[encodedLength] = '\0';
  
  // エンコード実行
  size_t actualLength = 0;
  mbedtls_base64_encode(encodedBuffer, encodedLength + 1, &actualLength, data, length);
  
  // 文字列に変換
  String result = String((char*)encodedBuffer);
  
  // メモリ解放
  free(encodedBuffer);
  
  return result;
}


// HMAC-SHA256署名の生成関数
static String generateSignature(const String& token, const String& secret, const String& timestamp, const String& nonce) {
  // 署名文字列の作成: token + timestamp + nonce
  String signaturePayload = token + timestamp + nonce;

  // HMACキーの設定
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
  
  // HMAC-SHA256ダイジェストの計算
  uint8_t hmacResult[32];
  mbedtls_md_hmac_starts(&ctx, (const unsigned char*)secret.c_str(), secret.length());
  mbedtls_md_hmac_update(&ctx, (const unsigned char*)signaturePayload.c_str(), signaturePayload.length());
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  
  // ダイジェストをBase64エンコード
  String signature = base64Encode(hmacResult, 32);
  
  // メモリの解放
  mbedtls_md_free(&ctx);
  
  return signature;
}

CallSwitchbot::CallSwitchbot() {
  // 乱数ジェネレータの初期化
  randomSeed(micros());
}

boolean CallSwitchbot::executeScene(const String& sceneId) {
  // タイムスタンプの取得（ミリ秒単位）
  unsigned long long epoch =  timeClient.getEpochTime();
  unsigned long long currentTime = epoch * 1000ULL;
  String timestamp = String(currentTime);

  // String timestamp = String(millis());

  // ノンス（リクエストID）の生成
  String nonce = "ESP32_" + String(random(10000)); 

  // 署名の生成
  String signature = generateSignature(API_TOKEN, API_SECRET, timestamp, nonce);
  
  // JSONペイロードの作成
  String jsonPayload = "{\"sceneId\":\"" + sceneId + "\"}";

  // HTTPクライアントの設定
  HTTPClient http;
  String url = BASE_URL + "/scenes/" + sceneId + "/execute";
  http.begin(url);
  
  // HTTPヘッダーの設定
  http.addHeader("Authorization", API_TOKEN);
  http.addHeader("sign", signature);
  http.addHeader("nonce", nonce);
  http.addHeader("t", timestamp);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Content-Length", String(jsonPayload.length()));

  // POSTリクエストの送信
  int httpResponseCode = http.POST(jsonPayload);
  
  // レスポンスの処理
  bool success = false;
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
    success = true;
  } else {
    Serial.println("Error sending command");
  }
  
  http.end();
  return success;
}

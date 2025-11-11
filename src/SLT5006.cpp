/**
 * @file SLT5006.cpp
 * @brief SLT5006土壌センサー用Arduinoライブラリ
 * @author Masafumi Horimoto
 * @version 1.1.0
 * @date 2025-11-10
 * @license MIT
 */

#include "SLT5006.h"

// センサーコマンド定義
static const byte CMD_CHECK_VER[] PROGMEM = {0x01, 0x00, 0x07, 0xc2, 0x61};
static const byte CMD_START_MEASURE[] PROGMEM = {0x02, 0x07, 0x01, 0x01, 0x0D, 0x70};
static const byte CMD_CHECK_MEASURE[] PROGMEM = {0x01, 0x08, 0x01, 0x00, 0xe6};
static const byte CMD_READ_RESULT[] PROGMEM = {0x01, 0x13, 0x10, 0xfc, 0x2c};

// コンストラクタ
SLT5006::SLT5006(uint8_t rxPin, uint8_t txPin) 
  : mySerial(rxPin, txPin) {
  // データ初期化
  data.temp = 0.0;
  data.ec_bulk = 0.0;
  data.vwc_rock = 0.0;
  data.vwc = 0.0;
  data.vwc_coco = 0.0;
  data.ec_pore = 0.0;
  data.rcode = 0;
}

// 初期化
void SLT5006::begin() {
  byte check_ver[5];
  
  // PROGMEMからコマンドをコピー
  memcpy_P(check_ver, CMD_CHECK_VER, 5);
  
  mySerial.begin(9600);
  delay(100);
  mySerial.write(check_ver, 5);
  rxData(0, 0);
  
  Serial.println(F("SLT5006 initialized"));
}

// データ要素変換
float SLT5006::dataConvEle(const byte* rdt, int h, int l, float f) {
  byte dth = rdt[h] & 0xff;
  byte dtl = rdt[l] & 0xff;
  int idt = dth * 0x100 + dtl;
  return (float)(idt * f);
}

// データ変換
void SLT5006::dataConv(const byte* rdt) {
  data.temp = dataConvEle(rdt, 4, 3, 0.0625);
  data.ec_bulk = dataConvEle(rdt, 6, 5, 0.001);
  data.vwc_rock = dataConvEle(rdt, 8, 7, 0.1);
  data.vwc = dataConvEle(rdt, 10, 9, 0.1);
  data.vwc_coco = dataConvEle(rdt, 12, 11, 0.1);
  data.ec_pore = dataConvEle(rdt, 16, 15, 0.001);
}

// データ受信
int SLT5006::rxData(int dataConvFlag, int completeCheck) {
  unsigned long startTime = millis();
  const long timeout = 500;  // タイムアウト500ms
  byte receiveData[64];
  int receivedBytes = 0;
  int ret = 0;
  
  // SoftwareSerialからデータが来るのを待つ
  while (millis() - startTime < timeout && receivedBytes < sizeof(receiveData)) {
    if (mySerial.available()) {
      receiveData[receivedBytes++] = mySerial.read();
    }
  }
  
  // 完了チェック
  if (completeCheck == 1) {
    if (receivedBytes > 3 && receiveData[3] == 1) {
      ret = 0;  // Measuring completed
    } else {
      ret = 2;  // Measuring not completed
    }
  } else {
    ret = 0;
  }
  
  // データ処理
  if (receivedBytes > 0) {
    //Serial.print(F("Receive from SoftwareSerial: ")); //
    //byteArrayToHexString(receiveData, receivedBytes); // debug
    
    if (dataConvFlag != 0) {
      dataConv(receiveData);
    }
  } else {
    // Serial.println(F("No data from SoftwareSerial")); // debug
    ret = 3;
  }
  data.rcode = ret;
  return ret;
}

// バイト配列を16進数文字列に変換してシリアル出力
void SLT5006::byteArrayToHexString(const byte* byteArray, int length) {
  for (int i = 0; i < length; ++i) {
    if (byteArray[i] < 0x10) {
      Serial.print(F("0"));
    }
    Serial.print(byteArray[i], HEX);
    Serial.print(F(" "));
  }
  Serial.println();
}

// センサーデータ読み込み
bool SLT5006::readSensor() {
  byte start_measure[6];
  byte check_measure[5];
  byte read_result[5];
  int r,k;
  
  // PROGMEMからコマンドをコピー
  memcpy_P(start_measure, CMD_START_MEASURE, 6);
  memcpy_P(check_measure, CMD_CHECK_MEASURE, 5);
  memcpy_P(read_result, CMD_READ_RESULT, 5);
  
  // 測定開始
  mySerial.write(start_measure, 6);
  r = rxData(0, 0);
  
  // 測定完了待ち
  k = 0;
  do {
    mySerial.write(check_measure, 5);
    r = rxData(0, 1);
    if (k>4) break;
    k++;
  } while (r == 2);
  
  // 結果読み込み
  mySerial.write(read_result, 5);
  r = rxData(1, 0);  // データ変換フラグ=1
  
  return (r == 0);
}

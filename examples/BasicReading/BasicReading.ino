/*
 * SLT5006 Basic Reading Example
 * 
 * このサンプルはSLT5006センサーから基本的なデータを読み取り、
 * シリアルモニタに表示します。
 * 
 * 配線:
 * Arduino A5 (RX) → SLT5006 TX
 * Arduino A4 (TX) → SLT5006 RX
 * Arduino 5V      → SLT5006 VCC
 * Arduino GND     → SLT5006 GND
 */

#include <SLT5006.h>

// センサーオブジェクト作成 (RX=A5, TX=A4)
SLT5006 sensor(A5, A4);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // シリアルポートの接続を待つ
  }
  
  Serial.println(F("SLT5006 Basic Reading Example"));
  Serial.println(F("==============================="));
  
  // センサー初期化
  sensor.begin();
  
  Serial.println(F("Sensor initialized"));
  Serial.println();
}

void loop() {
  Serial.println(F("Reading sensor..."));
  
  // センサーデータ読み込み
  if (sensor.readSensor()) {
    // 読み取り成功
    Serial.print(F("Temperature: "));
    Serial.print(sensor.getTemp(), 2);
    Serial.println(F(" °C"));
    
    Serial.print(F("EC Bulk: "));
    Serial.print(sensor.getECBulk(), 3);
    Serial.println(F(" mS/cm"));
    
    Serial.print(F("VWC: "));
    Serial.print(sensor.getVWC(), 1);
    Serial.println(F(" %"));
    
    Serial.println();
  } else {
    Serial.println(F("Failed to read sensor!"));
  }
  
  // 5秒待機
  delay(5000);
}


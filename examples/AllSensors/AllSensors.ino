/*
 * SLT5006 All Sensors Reading Example
 * 
 * このサンプルはSLT5006センサーのすべてのデータ（温度、EC、VWC）を
 * 読み取り、整形してシリアルモニタに表示します。
 */

#include <SLT5006.h>

// センサーオブジェクト作成 (RX=A5, TX=A4)
SLT5006 sensor(A5, A4);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // シリアルポートの接続を待つ
  }
  
  Serial.println(F("SLT5006 All Sensors Reading Example"));
  Serial.println(F("===================================="));
  
  // センサー初期化
  sensor.begin();
  
  Serial.println(F("Sensor initialized"));
  Serial.println();
  
  // ヘッダー表示
  printHeader();
}

void loop() {
  // センサーデータ読み込み
  if (sensor.readSensor()) {
    printSensorData();
  } else {
    Serial.print(F("ERROR: Failed to read sensor!  RC="));
    Serial.println(sensor.getRCode());
  }
  
  // 10秒待機
  delay(10000);
}

void printHeader() {
  Serial.println(F("Time(s)\tTemp(°C)\tEC_B(mS/cm)\tEC_P(mS/cm)\tVWC(%)\tVWC_R(%)\tVWC_C(%)"));
  Serial.println(F("-------\t--------\t-----------\t-----------\t------\t--------\t--------"));
}

void printSensorData() {
  // 経過時間（秒）
  Serial.print(millis() / 1000);
  Serial.print(F("\t"));
  
  // 温度
  Serial.print(sensor.getTemp(), 2);
  Serial.print(F("\t\t"));
  
  // EC Bulk
  Serial.print(sensor.getECBulk(), 3);
  Serial.print(F("\t\t"));
  
  // EC Pore
  Serial.print(sensor.getECPore(), 3);
  Serial.print(F("\t\t"));
  
  // VWC
  Serial.print(sensor.getVWC(), 1);
  Serial.print(F("\t"));
  
  // VWC Rock
  Serial.print(sensor.getVWCRock(), 1);
  Serial.print(F("\t\t"));
  
  // VWC Coco
  Serial.print(sensor.getVWCCoco(), 1);
  
  Serial.println();
}


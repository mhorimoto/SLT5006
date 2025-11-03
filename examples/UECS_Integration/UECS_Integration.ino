/*
 * SLT5006 UECS Integration Example
 * 
 * このサンプルはSLT5006センサーをUECS（ユビキタス環境制御システム）と
 * 統合する例です。センサーデータをUDPパケットで送信します。
 * 
 * 注意: このサンプルを実行するには、別途UECSライブラリが必要です。
 * uecsSendData関数は外部で定義されている必要があります。
 */

#include <SLT5006.h>
#include <avr/wdt.h>

// SLT5006センサーオブジェクト (RX=A5, TX=A4)
SLT5006 sensor(A5, A4);

// UECS送信用バッファ
char val[16];

// CCM形式定義 (環境に応じて変更してください)
#define CCMFMT "CCM.aM1"

// UECS送信関数のプロトタイプ宣言
// 実際の実装は別途用意する必要があります
extern void uecsSendData(int channel, char* xmlDT, char* val, int priority);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // シリアルポートの接続を待つ
  }
  
  Serial.println(F("SLT5006 UECS Integration Example"));
  Serial.println(F("================================="));
  
  // ウォッチドッグタイマー設定（8秒）
  wdt_enable(WDTO_8S);
  
  // センサー初期化
  sensor.begin();
  
  Serial.println(F("Setup complete"));
  Serial.println();
}

void loop() {
  char *xmlDT = CCMFMT;
  
  Serial.println(F("Reading sensor and sending UECS data..."));
  
  // センサーデータ読み込み
  if (sensor.readSensor()) {
    // 温度送信 (チャンネル1)
    dtostrf(sensor.getTemp(), -6, 3, val);
    uecsSendData(1, xmlDT, val, 0);
    Serial.print(F("Ch1 (Temp): "));
    Serial.println(val);
    
    // EC Bulk送信 (チャンネル2)
    dtostrf(sensor.getECBulk(), -6, 3, val);
    uecsSendData(2, xmlDT, val, 0);
    Serial.print(F("Ch2 (EC Bulk): "));
    Serial.println(val);
    
    // VWC Rock送信 (チャンネル3)
    dtostrf(sensor.getVWCRock(), -5, 1, val);
    uecsSendData(3, xmlDT, val, 0);
    Serial.print(F("Ch3 (VWC Rock): "));
    Serial.println(val);
    
    // VWC送信 (チャンネル4)
    dtostrf(sensor.getVWC(), -5, 1, val);
    uecsSendData(4, xmlDT, val, 0);
    Serial.print(F("Ch4 (VWC): "));
    Serial.println(val);
    
    // VWC Coco送信 (チャンネル5)
    dtostrf(sensor.getVWCCoco(), -5, 1, val);
    uecsSendData(5, xmlDT, val, 0);
    Serial.print(F("Ch5 (VWC Coco): "));
    Serial.println(val);
    
    // EC Pore送信 (チャンネル6)
    dtostrf(sensor.getECPore(), -6, 3, val);
    uecsSendData(6, xmlDT, val, 0);
    Serial.print(F("Ch6 (EC Pore): "));
    Serial.println(val);
    
    Serial.println(F("Data sent successfully"));
    Serial.println();
  } else {
    Serial.println(F("ERROR: Failed to read sensor!"));
  }
  
  // ウォッチドッグリセット
  wdt_reset();
  
  // 10秒待機
  delay(10000);
}


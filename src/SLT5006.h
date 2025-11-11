/**
 * @file SLT5006.h
 * @brief SLT5006土壌センサー用Arduinoライブラリ
 * @author Masafumi Horimoto
 * @version 1.1.0
 * @date 2025-11-10
 * @license MIT
 */

#ifndef SLT5006_H
#define SLT5006_H

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * @brief SLT5006センサーデータ構造体
 */
struct SLT5006DATA {
  float temp;       ///< 温度 (°C)
  float ec_bulk;    ///< EC bulk (dS/m)
  float vwc_rock;   ///< VWC rock (%) - ロックウール用
  float vwc;        ///< VWC (%) - 標準
  float vwc_coco;   ///< VWC coco (%) - ココピート用
  float ec_pore;    ///< EC pore (dS/m)
  int   rcode;      ///< 戻り値 (0:no error,2:not finish,3:no sensor)
};

/**
 * @brief SLT5006センサークラス
 */
class SLT5006 {
public:
  /**
   * @brief コンストラクタ
   * @param rxPin Arduinoの受信ピン（センサーのTXに接続）
   * @param txPin Arduinoの送信ピン（センサーのRXに接続）
   */
  SLT5006(uint8_t rxPin, uint8_t txPin);
  
  /**
   * @brief センサー初期化
   * setup()内で呼び出してください
   */
  void begin();
  
  /**
   * @brief センサーからデータを読み取る
   * @return 成功した場合true、失敗した場合false
   */
  bool readSensor();
  
  /**
   * @brief 温度を取得
   * @return 温度 (°C)
   */
  float getTemp() const { return data.temp; }
  
  /**
   * @brief EC Bulkを取得
   * @return EC bulk (mS/cm)
   */
  float getECBulk() const { return data.ec_bulk; }
  
  /**
   * @brief VWC Rockを取得
   * @return VWC rock (%)
   */
  float getVWCRock() const { return data.vwc_rock; }
  
  /**
   * @brief VWCを取得
   * @return VWC (%)
   */
  float getVWC() const { return data.vwc; }
  
  /**
   * @brief VWC Cocoを取得
   * @return VWC coco (%)
   */
  float getVWCCoco() const { return data.vwc_coco; }
  
  /**
   * @brief EC Poreを取得
   * @return EC pore (mS/cm)
   */
  float getECPore() const { return data.ec_pore; }
  
  /**
   * @brief Return Codeを取得
   * @return rcode
   */
  int getRCode() const { return data.rcode; }
  
  /**
   * @brief 全データを構造体で取得
   * @return SLT5006DATA構造体
   */
  SLT5006DATA getData() const { return data; }

private:
  SoftwareSerial mySerial;  ///< SoftwareSerialオブジェクト
  SLT5006DATA data;         ///< センサーデータ
  
  /**
   * @brief データ要素変換
   * @param rdt 受信データバイト配列
   * @param h 上位バイトのインデックス
   * @param l 下位バイトのインデックス
   * @param f 変換係数
   * @return 変換後の値
   */
  float dataConvEle(const byte* rdt, int h, int l, float f);
  
  /**
   * @brief 受信データを各センサーデータに変換
   * @param rdt 受信データバイト配列
   */
  void dataConv(const byte* rdt);
  
  /**
   * @brief センサーからデータ受信
   * @param dataConvFlag データ変換フラグ（0:変換しない, 1:変換する）
   * @param completeCheck 完了チェックフラグ（0:チェックしない, 1:チェックする）
   * @return ステータスコード（0:成功, 2:測定未完了, 3:データ受信不可）
   */
  int rxData(int dataConvFlag, int completeCheck);
  
  /**
   * @brief バイト配列を16進数文字列に変換してシリアル出力
   * @param byteArray バイト配列
   * @param length 配列長
   */
  void byteArrayToHexString(const byte* byteArray, int length);
};

#endif // SLT5006_H

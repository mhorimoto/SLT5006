# SLT5006 Arduino Library

村田製作所製SLT5006土壌センサー用のArduinoライブラリです。温度、EC（Bulk,Pore）、VWC（体積含水率）などのデータを簡単に読み取ることができます。

## 特徴

- 簡単なAPI設計
- SoftwareSerialを使用したUART通信
- 複数センサーの同時使用に対応
- 温度、EC、VWCなど6種類のデータ取得
- UECS（ユビキタス環境制御システム）との統合サンプル付き

## サポートするセンサーデータ

- **温度** (Temperature): °C
- **EC Bulk**: dS/m
- **VWC Rock**: % (ロックウール用)
- **VWC**: % (標準)
- **VWC Coco**: % (ココピート用)
- **EC Pore**: dS/m

## ハードウェア要件

- Arduino Nano / Uno / Mega など
- SLT5006 土壌センサー
- RS485通信モジュール（必要に応じて）

## 配線
```
Arduino → SLT5006
A5 (RX) → TX (センサー送信)
A4 (TX) → RX (センサー受信)
5V → VCC
GND → GND
```

## インストール (ここは、未対応)

### Arduino IDE経由

1. Arduino IDEを開く
2. **スケッチ** → **ライブラリをインクルード** → **ライブラリを管理...**
3. "SLT5006"で検索してインストール

### 手動インストール

1. このリポジトリをZIPでダウンロード
2. Arduino IDEで **スケッチ** → **ライブラリをインクルード** → **.ZIP形式のライブラリをインストール...**
3. ダウンロードしたZIPファイルを選択

### Gitから直接
```sh
cd ~/Documents/Arduino/libraries/
git clone https://github.com/yourusername/SLT5006.git
```

## 使い方

### 基本的な読み取り
```C++
#include <SLT5006.h>

// センサーオブジェクト作成 (RX=A5, TX=A4)
SLT5006 sensor(A5, A4);

void setup() {
    Serial.begin(9600);
    // センサー初期化
    sensor.begin();
}

void loop() {
    // センサーデータ読み込み
    if (sensor.readSensor()) {
        Serial.print("Temperature: ");
        Serial.print(sensor.getTemp());
        Serial.println(" °C");
        Serial.print("EC Bulk: ");
        Serial.print(sensor.getECBulk());
        Serial.println(" mS/cm");
        Serial.print("VWC: ");
        Serial.print(sensor.getVWC());
        Serial.println(" %");
    }
    delay(5000); // 5秒待機
}
```
### 複数センサーの使用
```C++
#include <SLT5006.h>

SLT5006 sensor1(A5, A4); // センサー1
SLT5006 sensor2(A3, A2); // センサー2

void setup() {
    Serial.begin(9600);
    sensor1.begin();
    sensor2.begin();
}

void loop() {
    if (sensor1.readSensor()) {
        Serial.print("Sensor1 Temp: ");
        Serial.println(sensor1.getTemp());
    }

    if (sensor2.readSensor()) {
        Serial.print("Sensor2 Temp: ");
        Serial.println(sensor2.getTemp());
    }
    delay(5000);
}
```

## API リファレンス

### コンストラクタ

SLT5006(uint8_t rxPin, uint8_t txPin)

- `rxPin`: Arduinoの受信ピン（センサーのTXに接続）
- `txPin`: Arduinoの送信ピン（センサーのRXに接続）

### メソッド

#### `void begin()`
センサーを初期化します。`setup()`内で呼び出してください。

#### `bool readSensor()`
センサーからデータを読み取ります。成功した場合は`true`を返します。  
エラーが発生した場合(戻り値が`false`)には、getRCode()でエラー内容を参照できます。

#### データ取得メソッド

- `float getTemp()` - 温度 (°C)
- `float getECBulk()` - EC Bulk (mS/cm)
- `float getVWCRock()` - VWC Rock (%)
- `float getVWC()` - VWC (%)
- `float getVWCCoco()` - VWC Coco (%)
- `float getECPore()` - EC Pore (mS/cm)
- `int getRCode()` - 戻り値 (0:正常,2:変換不可,3:未受信)
- `SLT5006DATA getData()` - 全データを構造体で取得

## サンプルスケッチ

ライブラリには以下のサンプルが含まれています：

- **BasicReading**: 基本的なセンサーデータの読み取り
- **AllSensors**: すべてのセンサーデータを表示
- **UECS_Integration**: UECS環境制御システムとの統合例

## トラブルシューティング

### データが読み取れない

1. SLT5006は活線挿抜不可です。電源を切ってから接続、取り外しをしてください。
2. 配線を確認してください。（特にRX/TXの接続）
3. センサーの電源供給を確認してください。
4. センサーとの接続ボーレートが9600bpsであることを確認してください。
5. 受信・変換に約3秒弱かかります。

### タイムアウトエラー

- センサーの応答時間は最大500msです。
- 複数センサーを使用する場合は、読み取り間隔を十分に確保してください。

## ライセンス

MIT License

## 作者

堀本　正文 mh@ys-lab.tech

## 貢献

プルリクエストを歓迎します。大きな変更の場合は、まずissueを開いて変更内容を議論してください。

## 参考資料

- [SLT5006 データシート](https://www.chip1stop.com/products/Murata-Manufacturing/SLT5006/MURA*0161701)
- [UECS公式サイト](https://www.uecs.jp/)

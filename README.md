ESP32 - FreeRTOSを使ったサンプルコード
====

 ESP32が搭載された開発ボードを使い、
 FreeRTOSで動作するサンプルコード置き場です。
 電子工作でよく使うパターンを載せていきます。

## Description

- SampleSw
  - タクトスイッチを使って、スイッチイベントを検出します
  - 詳細は、[Qiita記事 - ESP32－FreeRTOSにて、スイッチをイベントを検出する](https://qiita.com/kotaproj/items/e9b2f56a89ff0a6a3d3a)参照のこと 
  - コードは、[スイッチ検出](https://github.com/kotaproj/esp32Samples/tree/master/SampleSw)
- SampleRe
  - ロータリーエンコーダを使って、回転イベントを検出します
  - 詳細は、[Qiita記事 - ESP32－FreeRTOSにて、ロータリーエンコーダのイベントを検出する](https://qiita.com/kotaproj/items/429c79c810e96158c583)参照のこと 
  - コードは、[回転の検出](https://github.com/kotaproj/esp32Samples/tree/master/SampleRe)

## Requirement

* windows10 64bit
* Arduino IDE 1.8.10
  * Esplora Built-In by Arduino 1.0.4
  * FreeRTOS by Richard Barry 10.2.0-3
  * RotaryEncoder by Matthias Hertel 1.3.0
    * [Github - RotaryEncoder](https://github.com/mathertel/RotaryEncoder)
* vscode 1.41.1
  * PlatformIO - Home 3.0.1, Core 4.1.0

## Usage

* 各プロジェクトごとの接続を確認のこと
  * [スイッチ検出](https://github.com/kotaproj/esp32Samples/tree/master/SampleSw)
  * [回転の検出](https://github.com/kotaproj/esp32Samples/tree/master/SampleRe)

## Install

## Contribution

## Licence

[MIT](https://github.com/tcnksm/tool/blob/master/LICENCE)

## Author

[kotaproj](https://github.com/kotaproj)


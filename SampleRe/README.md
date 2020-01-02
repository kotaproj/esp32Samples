ESP32 - FreeRTOSを使ったサンプルコード - ロータリーエンコーダ処理
====

 ESP32 - FreeRTOSを使った
 ロータリーエンコーダ処理のサンプルコードです。
 
## Description

 詳細は、Qiita記事に記載予定です。
 <!-- 詳細は、[Qiita記事 - ESP32－FreeRTOSにて、スイッチをイベントを検出する](https://qiita.com/kotaproj/items/e9b2f56a89ff0a6a3d3a)にまとめてあります。 -->

## Requirement

* windows10 64bit
* Arduino IDE 1.8.10
  * Esplora Built-In by Arduino 1.0.4
  * FreeRTOS by Richard Barry 10.2.0-3
  * RotaryEncoder by Matthias Hertel 1.3.0
    * [Github - RotaryEncoder](https://github.com/mathertel/RotaryEncoder)
* vscode 1.41.1
  * PlatformIO - Home 3.0.1, Core 4.1.0

本コードは、RotaryEncoderライブラリを使用しています。<br>
./lib下に、RotaryEncoderのライブラリを配置する必要があります。<br>
例：
```
 ./SampleRe/lib/RotaryEncoder-master/RotaryEncoder.cpp
 ./SampleRe/lib/RotaryEncoder-master/RotaryEncoder.h
```

## Author

[kotaproj](https://github.com/kotaproj)


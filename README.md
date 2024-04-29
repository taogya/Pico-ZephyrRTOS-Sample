# Pico-ZephyrRTOS-Sample
Zephyr RTOSを使用した、Raspberry Pi Picoのファームウェア<br>

## 環境構築
Zephyr RTOSのサンプルを動かせるようにする。<br>
https://taogya.hatenablog.com/entry/20240423/1713802695

zephyrprojectのフォルダは以下としています。<br>
`~/develop/zephyrproject`

## ビルド方法
```sh
$ sh build.sh
```

## 書き込み方法
Picoprobeを持っていないので、直接書き込みます。<br>
pico のブートローダースイッチを押しながらPCへ接続します。<br>
```sh
$ sh flash.sh
```
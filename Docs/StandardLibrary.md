# 設計ノート

## 静的ライブラリ/動的ライブラリ
### 静的ライブラリ
* dllexportする必要がない
  * メンバにstdのクラスを含めることができる

### 動的ライブラリ
* 複数のプログラムでライブラリを共有することができる
  * プログラムの軽量化
  * 共有しない場合(1つのexeで実行する場合)は軽量化しない

## 用語
|名前|意味
|---|---|
|OctbitEngine|システム全体の名称|
|OctbitEditor|エディタ|


* Sources
  * Engine
    * Foundation
    * Core
    * Platform
    * Input
    * Framerwork
  * Editor
  * Launcher
Graphicモジュール
====================

## 概要
GraphicモジュールではDirectXやVulkanなどの異なる描画APIの共通化を行います。

## 構成
### System
異なる描画APIを生成するシングルトンです。
起動時に描画APIを指定することで対応するグラフィック・モジュールを読み込み、各描画APIで実装されたDeviceを取得します。

### Device
グラフィック・オブジェクトを生成するファクトリクラスです。
実装はプラグインで行い、各種グラフィック・オブジェクトを実装した派生オブジェクトを生成します。
Device事態はGraphicModuleから生成されます。

## 使用例
プラットフォーム(OS)ごとに起動方法が異なるため、識別マクロを使用して個別の起動処理を実装する必要があります。
```c++
graphic::System::Ref()::startup(GraphicAPI::DirectX);
auto pDevice=graphic::System::Ref()::getDevice();
auto buffer=pDevice->createBuffer(0x1000,TC("TestBuffer"));
```

## リソース
リソースは大別して三種類あります。
### Read
* 頂点
* インデックス
* テクスチャ
### ReadWrite
* レンダーターゲット
### ReadWrite
* Unoredered Access
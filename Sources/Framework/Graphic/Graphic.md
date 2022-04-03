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

## オブジェクト
### SwapChain
ウィンドウと結びつけてディスプレイに表示するための機構。
内部にテクスチャバッファを複数持っており、表示中のテクスチャバッファに書き込まれないようにする。
### Texture
色の集合体
### RenderTexture
内部的に複数のカラーテクスチャと0または1つのデプステクスチャを持つ。
### Shader
入力された頂点/テクスチャをもとに出力を決定する。
### RootSignature
入力するリソース(テクスチャ/定数バッファ)の設定(可視性/スロット)。
どのリソースをまとめて設定できるかを定義。
### PipelineState
描画の入力/処理/出力を定義する。
* 入力
	* 頂点レイアウト
	* トポロジー・タイプ
* 処理
	* シェーダ
	* 合成方法
	* ラスタライズ
	* カリング
	* ワイヤーフレーム
* 出力
	* デプス・ステンシルの書き込み方法
	* 出力カラーフォーマット
	* 出力デプスフォーマット
### RenderPass
利用される/生成されるリソースを処理ごとに定義してそれらの依存性をまとめたもの
Subpass/Attachment/Dependency
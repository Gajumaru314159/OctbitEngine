RHIモジュール
====================

## 概要
RHIモジュールではDirectXやVulkanなどの異なる描画APIの共通化させるためのインターフェイスを定義します。

インターフェイスの定義のみをするため、描画パイプラインやカメラの定義などは実装しません。それらはより上位のRPI層で実装されます。

## クラス説明

### RHI
グラフィック・オブジェクトを生成するファクトリクラスです。

```Texture::Create()```や```Buffer::Create()```のような生成ユーティリティは内部的にRHIを通してオブジェクトの生成を行います。
複数のRHIの実装を同時に使用してはいけません。(DirectX12とVulkanの併用など)

### Buffer
### CommandList
### DescriptorTable
### Display
### FrameBuffer
### PipelineState
### RenderPass
### RenderTexture
### RootSignature
### Shader
### Texture
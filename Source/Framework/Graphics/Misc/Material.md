# Material {#Material}

Materialは内部に複数のPassを持ちます。(例:EarlyZ/Opacity)  
複数のPassがある場合でもプロパティは共通しています。
* Material
	* Property[]
		* Name
		* Value
	* Pass[]
		* Map<RenderTag,Subpass>
		* Map<ShaderStage,Shader>

## 変数のスコープ
変数を共有する単位で分けて管理します。

|名前|管理者|使用例|
|----|--------|---|
|グローバル変数|Graphics|時間|
|マテリアル変数|Material|マテリアル毎のパラメータ|
|シーン変数|RenderScene|ライト情報|
|ビュー変数|RenderView|カメラ行列|
|インスタンス変数|RenderUnit|インスタンシングのTransform配列|

## RenderPipelineからみたMaterial

## MaterialPass
MaterialPassはEarlyZやOpacityなど、同一のモデルを異なる方法で描画するための仕組みです。

Materialは異なる頂点レイアウトのモデルの描画をサポートするために、最低限必要な頂点レイアウト(InputLayout)を設定します。

描画に必要な情報として、RenderTargetのフォーマットやシェーダー、ブレンド方法などの設定も必要です。

```cpp
struct MaterialPass {
	String name;

    Vector<InputLayout>				inputLayout;
	
	rhi::RenderTargetFormatArray	colors;
    Optional<rhi::TextureFormat>	depth;
    rhi::BlendDescList		        blends;
    rhi::RasterizerDesc		        rasterizer;
    rhi::DepthStencilDesc	        depthStencil;

	Ref<DescriptorLayout>			globalLayout;
	Ref<DescriptorLayout>			sceneLayout;
	Ref<DescriptorLayout>			viewLayout;
	Ref<DescriptorLayout>			instanceLayout;
};
```

## MaterialVariant

## MaterialLOD
> [!warning]
> MaterialVariantと設計を統合予定です。MaterialLODではShaderを指定せずに、どのMaterialVariantを使用するかを指定する予定です。

MaterialLODは描画距離やグラフィックスクオリティに応じて、異なるシェーダーを使用するための仕組みです。これにより、パフォーマンスを最適化しつつ、視覚的な品質を維持することができます。
ただし、マテリアルのパラメータはLOD間で共有されるため、LODごとに異なるシェーダーを使用する場合でも、同じマテリアルパラメータを使用できます。
```cpp
struct Shader {
	Ref<ShaderProgram> vs;
	Ref<ShaderProgram> ps;
};

struct MaterialLOD
{
	f32 quality = 1;
	Shader shader;
};

struct MaterialPass {
	Vector<MaterialLOD> lods;
    ...
};


```


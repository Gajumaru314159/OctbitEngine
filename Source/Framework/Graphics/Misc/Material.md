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
struct ShaderSet {
	Ref<ShaderProgram> vs;
	Ref<ShaderProgram> ps;
};

struct MaterialLOD
{
	f32 quality = 1;
	ShaderSet shader;
};

struct MaterialPass {
	Vector<MaterialLOD> lods;
    ...
};


```


```cpp

void selectVariant() {

	f32 distance = 100.0f;
	f32 quality = 1.0f;

	// レベル計算はプロっジェクト独自にオーバーライド可能
	s32 level = calcLevel(distance,quality);

	level = min(level,pass.lodMax);

	ShaderKeywords keywords = { Format("SHADER_QUALITY_{}",level) };
	keywords.union(pass.keywords);
	keywords.union(customKeywords);

	auto& shaders = pass.shaders.at(keywords);
	
	// RootSignatureとPipelineStateの生成はGameThreadで行ってはいけない。
	// StreamingSystemと同じように非同期で生成要求をかけ、準備できたら切り替える。
	// 読み込み済みだったら即時反映できるようにリクエスト自体はGameThreadで行いたい
	PipelineStateDesc desc;
	desc.vs = shaders.vs;
	desc.ps = shaders.vp;
	desc.rasterizer = pass.rasterizer;

	PipelineState::Create(desc);

}

using ShaderKeywordSet = HashSap<Name>;
using ShaderKeywordMap = HashMap<Name,bool>;
using ShaderMap = HashMap<ShaderKeyrordSet,ShaderSet>;


struct ShaderSet {
	Ref<ShaderProgram> 		vs;
	Ref<ShaderProgram> 		ps;
	// Shader毎にブレンド方法までカスタムするならPipelineState関連の情報はこっち (データが大きくなりそうならindexで指定するようにして共有できるようにする)
	
	// PipelineState周りの必須情報
    Vector<InputLayout>		inputLayout;
	RenderTargetFormatArray	colors;
    Optional<TextureFormat>	depth;
    BlendDescList		    blends;
    RasterizerDesc		    rasterizer;
    DepthStencilDesc	    depthStencil;
};

// MaterialPassも言ってしまえばShaderVariantの1つ
// 付加情報として描画パス名が増える
// 後はスキニングとそれ以外で頂点レイアウトが変わるかも？
struct MaterialPass {
	String name;
	ShaderKeywordSet	keywords;	// RenderPassのキーワード RENDER_PASS_EARLY_Z
	s32					lodNum;		// SHADER_QUALITY_0
	s32					lodMax;		// 最低限保証する品質 (遠景で使用している単色シェーダーは低品質にしたとしても使ってはいけないなど)
};
using MaterialPassMap = HashMap<Name,MaterialPass>;

struct MaterialDesc{
	String          name;
	
	Vector<String>  textures;
	Vector<String>  buffers;
	Vector<String>  matrices;
	Vector<String>  colors;
	Vector<String>  vectors;
	Vector<String>  scalars;
	
	// システム依存であればここで指定する必要はない(内部でマネージャーから取得する)
	// Ref<DescriptorLayout>	globalLayout;
	// Ref<DescriptorLayout>	sceneLayout;
	// Ref<DescriptorLayout>	viewLayout;
	// Ref<DescriptorLayout>	instanceLayout;
	// Ref<DescriptorLayout>	materialLayout;

	MaterialPassMap	passes;	
	ShaderMap 		shaders;	// 対応するものがなければエラーシェーダーにフォールバック
};



```


```csharp
void Compile(string code,int shaderQualityMax,string[] keywords) {

	for(int shaderQuality = 0; shaderQuality < shaderQualityMax) {

		foreach(string activeKeywords in keywords.Combination()) {
			
			List<string> args;

			args.Add($"-D SHAREDR_QUALITY={shaderQuality}");

			foreach(string keyword in activeKeywords) {
				args.Add($"-D {keyword}");
			}

			Compiler.Compile(code,args);
		}

	}

}

```

```c
float4 color = albedo.Sample(i.uv);

#if 0 < SHADER_QUALITY
color *= detail.Sample(i.uv*detailScale);
#endif

#if LIGHTING_ENABLED
color.xyz *= dot(i.normal,lightDir) *0.5 * 0.5;
#endif

o.color = color;
```
# Material

Materialは内部に複数のPassを持つ。  
複数のPassがある場合でもプロパティは共通。

```
Material
	ShaderCode
	Property[]
		Name
		Value
	Pass[]
		RenderTag -> RenderPass/Subpass
		Map<ShaderStage,EntryPoint>

```
通常はビルド済みのシェーダバイナリを使用するので以下のような構造となる。
```
Material
	Property[]
		Name
		Value
	Pass[]
		RenderTag -> RenderPass/Subpass
		Map<ShaderStage,Shader>
```

## ローカル変数とグローバル変数
* ローカル変数　：マテリアルごとに設定できる値
* グローバル変数：全てのマテリアルで共通の値
```cpp
Material::SetGlobalFloat(TC("Time"),Time::Now().toSeconds());
Material::SetGlobalColor(TC("LightColor"),Color(1,1,0.8f));
```
内部的には新しくグローバル変数が追加されるたびにインデックスが追加される。
```cpp
void SetGlobalFloat(StringView name,f32 value){
	if(m_scalarNames.count(name)==0){
		m_scalarNames.emplace_back(name,m_scalarNames.size());
	}
	m_scalars[m_scalarNames[name]]=value;
}
```
マテリアルで使用する場合は生成時にインデックスが渡される
```hlsl
g_scalars[index];
```
TimeなどのBuilt-in変数はインデックスではなく専用の構造体で渡す。

## インスタンス変数
モデル行列のようなインスタンスごとの値はインスタンス変数として定義される。  
マテリアル毎にMaterialInstancePropertyとして設定すると一度の描画命令で異なる情報を描画できる。
```hlsl
struct InstanceProperties{
	float4x4	matrix;
	float4		color;
}
cbuffer Buffer : register(b0) {	
	InstanceProperties ips[1024];				
};
```


## カメラ変数
カメラ行列やビュー行列など

|名前|設定回数(フレーム内)|
|----|--------|
|グローバル変数| 1回|
|カメラ変数| カメラ数 |
|ローカル変数| マテリアル数 |
|インスタンス変数| マテリアル数 |


## オブジェクトパラメータ
* モデル行列はインスタンシングのため

## RenderPipelineからみたMaterial
* RenderTagから描画するマテリアルを取得
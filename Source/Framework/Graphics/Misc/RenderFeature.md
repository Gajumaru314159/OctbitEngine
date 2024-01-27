# RenderFeature
RenderFeatureは付け外し可能な描画機能のモジュールです。  
RenderFeature内では描画機能の提供のみを行い、実際に描画に使用するかはRenderPipelineで決定します。
Engine層から使用する場合はComponentからRenderFeatureにアクセスし描画要素を生成します。

```
MeshRenderFeature
ImGuiRenderFeature
LightRenderFeature
TerrainRenderFeature
VFXRenderFeature
UIRenderFeature
ReflectionCubeRenderFeature
ShadowRenderFeature
```
アクセス例
```cpp
class MeshRendererComponent : public Component {
public:
	void onActivate(){
		if(auto feature = MeshRenderFeature::Of(getEntity())){
			m_meshId = feature->createItem();
		}	
	}
	void onDisactivate(){
		if(auto feature = MeshRenderFeature::Of(getEntity())){
			feature->releaseItem(m_meshId);
		}	
	}
	void setMesh(const Ref<Mesh>& mesh){
		if(auto feature = MeshRenderFeature::Of(getEntity())){
			feature->setMesh(m_meshId,mesh);
		}	
	}
};
```

## RenderFeatureの依存関係
RenderFeatureの中には描画機能を他のRenderFeatureに任せている場合があります。
* MeshRenderFeatureはMaterialRenderFeatureに依存

RenderPipelineからはMaterialRenderFeatureを実行するだけでMeshRenderFeatureに関連する描画が実行されます。


## エディタ上のワークフロー
Unityのワークフローを参考にしてください
1. RenderPipelineAssetを作成
2. RenderPipelineのパラメータを設定
3. RenderPipelineAssetのリストをGraphicsSetting設定
4. Cameraの使用するRenderPipelineを設定

## ポストプロセスの実行順序
ベースのRenderStage(PostProcess)からのオフセットで制御されます。
Materialで制御してください。
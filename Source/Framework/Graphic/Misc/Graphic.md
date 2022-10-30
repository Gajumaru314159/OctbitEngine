Graphic
=========

構成要素
----------
* Camera
* Model
* Material
* Cubemap
* FrustumCulling
* Renderer
* Light
* LOD
* Particle
* Decal
* Skybox
* Terrein


概要
----


```c++

Material material(shader);
material.setFloat("Scale",12.0f);
context.setMaterial(material);
context.draw(mesh);

```

# いつPipelineStateをつくるか
## 必要な情報
* 書き込み時に決まるもの
    * 書き込み先フォーマット
	* 頂点レイアウト
* 事前決定
	* ルートシグネチャ
	* シェーダ
	* ブレンドモード
	* ラスタライズ
	* デプス


```
Material
	MaterialPass[]
		Shader[]
		Tag
	Buffer
	Texture[]
```
* プロパティのBufferとトランスフォームのバッファは別になる
* トランスフォーム/色→モデルごと
* 



# Shader
## HLSLレベルに追加される概念
* マルチパス
* 描画タグ
* サブシェーダ(フォールバック)
* ブレンドモード


# 定義済み変数
```
// 共通
時間
フォグ

// モデル
モデル行列

// カメラ
ビュー行列
プロジェクション行列
スクリーンサイズ
```

# 描画パス
```c++
class RenderPipeline{
public:
	RenderPipeline(const Camera& camera)
		: base(camera)
	{
		// RT生成
		m_gbufferRT = camera.createRT();
		m_accumulateRT = camera.createRT();
		m_downsampleRT = camera.createRT();		
	}

	void render(RenderContet& context){
		// カメラごとのRTをつくる必要がある
		// サイズの更新はどこでする？

		// Unityではカメラのリストを受け取るが理由は？

		// マルチスレッドで描画するには？
		
		// 複数カメラの依存関係は？
		//		カメラの描画先のレンダーターゲットは1フレーム遅延(描画完了後にSwap)

		switch(camera.getType()){
		case CameraType::Reflection:
		case CameraType::Voxelize:
			break;
		default:
			// Rendererを描画する場合
			context.setCamera(camera);
			context.draw();
			// 独自コマンドを実行する場合(PostProcess)
			PostProcessManager::Instance().setPostProcess(camera);
			m_cmdBuffer.clear();
			m_cmdBuffer.drawMesh();
			// 最終描画先はcameraに設定されている
			m_cmdBuffer.copyTexture(m_accumulateRT,getFinalRT());
			context.executeCommandBuffer(m_cmdBuffer);
			// 実行
			context.submit();
			break;
		}
	}

private:
	CommandBuffer m_cmdBuffer;
	RenderTexture m_gbufferRT;
}


```
* 描画のソート単位はDrawItem
* DrawItem::render()
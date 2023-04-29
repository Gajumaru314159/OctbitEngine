# RenderFeature
FeatureRenderPipelineは取り外し可能な描画機能を組み合わせることで描画パイプラインを構築します。  
各RenderFeatureは複数のRenderPassと複数の1次テクスチャを定義します。


## クラス構成
* Graphics
	* UniversalRenderPipeline
	* Camera[]
		* ScriptableRenderer
			* ScriptableRendererFeature[]
			* ScriptableRenderPass[]

## クラス説明
|クラス|機能|
|-|-|
|UniversalRenderPipelie|ScriptableXXX系の機能を使えるようにしたRenderPipeline|
|Camera|カメラごとに使用するScriptableRendererを設定できる|
|ScriptableRenderer|ToonRendererやPBRRendererなど1カメラに対する一連の描画パイプライン<br>ScriptableRenderPassの配列を持つ|
|ScriptableRendererFeature|着脱可能な描画機能(Tonemap/Vintage)<br>必要なScriptableRenderPassを生成|
|ScriptableRenderPass|rhi::RenderPass単位の描画処理|

## 実装例
```c++

class UniversalRenderPipeline : public RenderPipeline {
public:
	void render(RenderContext& contex, Span<Ref<Camera>> cameras) {

		for(auto& feature:m_postFeatures){
			feature->execure(contex);
		}

		for(auto& camera:cameras){
			camera->renderer->execute(contex,camera);
		}

	}
};

```

```c++
class PRBRenderer: public ScriptableRenderer{
public:
	PRBRenderer(){
		m_passes.clear();
		for(auto& feature:m_features){
			feature->addRenderPass(m_passes);
		}
		std:sort(m_passes.begin(),m_pases.end(),
			[](const auto& a,const auto& b){
				return a->getPriority() < b->getPriority();
			}
		);
	}
	void execute(RenderContext& context,Camera& camera) {
		for(auto& pass:m_passes){
			pass.execute(context,camera,*this);
		}
	}
private:
	Array<UPtr<ScriptableRenderFeature>> m_features;
	Array<UPtr<ScriptableRenderPass>> m_passes;
}

```

```c++
class BlurRenderPass : public ScriptableRenderPass{
public:
	BlurRenderPass() {

		auto accumulate = manager.getTexture("Accumulate");

		m_material1 = Asset::Load<Material>(TC("System/Blur1"));
		m_material2 = Asset::Load<Material>(TC("System/Blur2"));

		RenderPassDesc rdesc;
		auto renderPass = manager.getRenderPass(rdesc);

		FrameBufferDesc fdesc;
		fdesc.renderPass = renderPass;
		fdessc.attachment = {
			gbuffer0,
			gbuffer1,
			gbuffer2,
			depth,
		};

		m_frameBuffer = FrameBuffer::Create(fdesc);
	}

	void execute(RenderContext& context,Camera& camera,Renderer& renderer){

		context.beginRenderPass(m_frameBuffer);
		context.blit(accumulate,m_passTex,m_material1);

		context.nextSubpass();
		context.blit(m_passTex,accumulate,m_material2);
	}
private:
	Ref<FrameBuffer> m_frameBuffer;
	Ref<Material> m_material1;
	Ref<Material> m_material2;
};
```
```c++
class OpaqueRenderPass : public ScriptableRenderPass{
public:
	OpaqueRenderPass(TextureManager& manager) {
		
		auto gbuffer0 = manager.getTexture("GBuffer0",TextureFormat::RGB8);	// Diffuse	Flag
		auto gbuffer1 = manager.getTexture("GBuffer1",TextureFormat::RGB8);	// Metallic	Occlusion
		auto gbuffer2 = manager.getTexture("GBuffer2",TextureFormat::RGB8);	// Normal	Smoothness
		auto depth    = manager.getTexture("Depth"   ,TextureFormat::D32);	// Depth

		RenderPassDescHelper rdesc;
		rdesc.name = TC("Forward");
		auto gbuffer0Idx = rdesc.addAttachment(TextureFormat::RGBA8);
		auto gbuffer1Idx = rdesc.addAttachment(TextureFormat::RGBA8);
		auto gbuffer2Idx = rdesc.addAttachment(TextureFormat::RGBA8);
		auto depthIdx = rdesc.addAttachment(TextureFormat::D32);
		rdesc.addSubpassXCD({ gbuffer0Idx,gbuffer1Idx,gbuffer2Idx }, depth);

		FrameBufferDesc fdesc;
		fdesc.renderPass = Material::AddRenderPass(rdesc);
		fdessc.attachment = {
			gbuffer0,
			gbuffer1,
			gbuffer2,
			depth,
		};

		m_frameBuffer = FrameBuffer::Create(fdesc);
	}

	void execute(RenderContext& context,Camera& camera,Renderer& renderer) {

		context.beginRenderPass(m_frameBuffer);

		context
			.draw("DepthPrepass")
			.cull()
			.sort();
	}
private:
	Ref<FrameBuffer> m_frameBuffer;
};
```

## RenderQueue
|名前|値|対象|
|-|-|-|
|Background|1000|背景|
|Geometry|2000|不透明|
|AlphaTest|2450|アルファテスト|
|Transparent|3000|半透明|
|Overlay|4000||

### テクスチャ/FrameBufferの生成
* 再生成が必要
	* Texture
	* FrameBuffer
* 共通
	* RenderPass
	* Material
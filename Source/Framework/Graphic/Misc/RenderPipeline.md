RenderPipeline
---------------
1フレーム中の描画処理にはカメラ描画前の共通処理とカメラごとの描画処理が必要。
カメラ毎にRenderPipelineを設定可能だが、そのRenderPipelineが共通処理で描画されたTextureに依存する場合がある。
その場合は何らかの方法で共通処理の結果を受け取る必要がある。

### CameraStack
別カメラの映像を背景を描画せずにオーバーレイさせるシステムです。  
CameraStackに積まれたカメラの描画はPostProcessとの依存関係を考慮して描画されます。
オーバーレイ描画のポストプロセスはベースカメラと同一です。

### カメラごとのRenderPipeline
PBRとNPBRを両方使用する場合はカメラごとにRenderPipelineを構築する必要があります。  
RenderPipelineが異なる場合RenderPassも異なるのでMaterialはPRBとNPRBの両方に対応する必要があります。
具体的にはRenderTagに```PBR_Opaque```と```NPBR_Opaque```それぞれのシェーダを含める必要があります。
```cpp
if(IsPBR(camera)){
	context.getRenderers(TC("PBR_Opaque"))
		.draw();
	// ...
}else{
	context.getRenderers(TC("NPBR_Opaque"))
		.draw();
	// ...
}
```

### Camera情報
一般的なパラメータはCameraコンポーネントが持つ。
カスタムRenderPipelineで必要なパラメータはCameraと同じEntityにコンポーネントとして追加する。


### Engine実装とGame実装
RenderPipelineにはシステム予約してあるRenderTagの描画を用意しておく。  
Engine実装で実現できないものがある場合は別のRenderTagを用意し個別実装する。
* ```drawShadow()```
* ```drawSky()```
* ```drawUI()```
* ```drawWireframe()```
* ```drawGizmo()```

### GlobalRenderPipeline

```cpp

class GlobalRenderPipeliene{
public:
	// システム的なGPGPU処理はrender前に実行される
	// IMGUIなどのデバッグ描画はrender後に実行される
	void render(RenderContext& context, Span<Camera> cameras) override {
		
		// カメラ由来では内処理(例：GI/Sea)
		{
			auto commandBuffer = context.allocateCommandBuffer();
			commandBuffer.dispatchCompute(giShader);
			context.executeCommandBuffer(commandBuffer);
		}

		// Camera.depthが小さい順に描画
		// RenderTextureの描画順はユーザ制御
		for(auto& camera:cameras){
			
			context.setCamera(camera);
			camera.renderPipeline(context,camera);

		}
	}

};

class ToonRenderPipeliene{
public:
	// システム的なGPGPU処理はrender前に実行される
	// IMGUIなどのデバッグ描画はrender後に実行される
	void render(RenderContext& context, Camera& camera) override {
		
		// カメラパラメータ取得
		CustomCameraParam param{};
		if(auto comp = camera.entity().findComponent<CameraDataComponent>()){
			param = comp.param();
		}
		
		// 内部でViewportの設定など
		context.setCamera(camera);

		// 描画
		context.draw();

		// PostProcess
		{
			auto commandBuffer = context.allocateCommandBuffer();
			commandBuffer.beginRenderPass(blur0);
			commandBuffer.blit(material,TC("BlurPass0"));
		}

		// カメラの描画対象にコピー
		context.blit(blur0,camera.target);

	}

};

```



```cpp

class SampleRenderPipeliene{
public:
	void render(RenderContext& context, Span<Camera> cameras) override {
		
		// カメラ由来では内処理(例：GI/GPGPU)


		// Camera.depthが小さい順に描画
		// RenderTextureの描画順はユーザ制御
		for(auto& camera:cameras){
			
			// 内部でViewportの設定など
			context.setCamera(camera);

			// 描画
			context.draw();

			// PostProcess
			{
				auto commandBuffer = context.allocateCommandBuffer();
				commandBuffer.beginRenderPass(blur0);
				commandBuffer.blit(material,TC("BlurPass0"));
			}

			// カメラの描画対象にコピー
			context.blit(blur0,camera.target);
		}
	}

}

```

## Rendererの描画
RendererはDrawContext内部にRenderTag毎に登録されている。

1. 特定のRenderTagを持つRendererを集める ※複数指定可
2. 描画条件に一致するものだけにフィルタ ※カリング
3. 描画順にソート ※距離順

## RenderスレッドとGameスレッド

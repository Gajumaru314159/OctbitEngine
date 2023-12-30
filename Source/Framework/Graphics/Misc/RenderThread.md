RenderThread
------------
描画は以下の3つのスレッドから構成されます。
* GameThread
* RenderThread
* RHIThread
* 
## GameThread
フレーム内の所定のタイミングで描画に必要なGPUリソースをRenderThread用にコピーします。

* 描画する可能性のある全てのリソースをSwapする
    * RenderThreadでRenderPipelineを処理する
* 描画アイテム単位でパケットを生成
* 描画コマンドのラッパーをスタック
    * RenderThreadではソートとネイティブのコマンド変換
```cpp
graphics.addTask(
	[this](Recorder& recorder){
		auto* resources = recorder.alloc<DrawItem>();
		resources.pipeline = pipeline;
		resources.vertex = vertex;
		recorder.addCommand(RenderTag::Opaque,&RenderTask,resources);
	}
);
```
## RenderThread
CommandListへコマンドを積む処理です。
```cpp
void RenderTask(CommandList& cmdList,void* args){
	auto resources = reinterpret_cast<Resources>(args);
	cmdList.setPipelineState(resources->pipeline);
	cmdList.setVertexBuffer(resources->vertex);
	cmdList.setIndexBuffer(resources->index);
}
```
## RHIThread
RenderThreadで積まれたRHIコマンドを実行するスレッドです。
```cpp
cmdlist.end();
cmdlist.flush();
cmdlist.begin();
```

Graphics
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


## 定義済み変数
* 共通
	* 時間
	* フォグ
* モデル
	* モデル行列

* カメラ
	* ビュー行列
	* プロジェクション行列
	* スクリーンサイズ


## RenderPipelineとWorldの関係
* World毎に表示物が完全に分かれる
    * WorldのサービスとしてRenderPipelineが存在する

* Material
    * GlobalParamはWorldを超えて使用される

```c++

RenderSceneDesc desc;
auto scene = RenderScene::Create(desc);
scene->enableFeature<MeshRenderFeature>();

scene->setRenderPipeline<UniversalRenderPipeline>();

auto view = scene->createView();

view->addDrawItem();

// Display
auto renderTexture = RenderTexture::Create();
view->setRenderTexture(renderTexture);

```
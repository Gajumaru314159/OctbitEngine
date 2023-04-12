# Scene

## 構成
* Engine
    * World[]
    	* Scene
    		* Entity[]
    			* Component[]
            * Scene[]

## World
* 通常1つだけ存在します。エディタから実行される場合は複数生成されます。
* 異なるWorld同士で情報をやり取りすることはできません。
	* RenderTextureを経由することで別Worldのテクスチャに描画することが可能です。
* Worldごとのシステムを持ちます
    * ShadowManager
	* GI

## Scene
* Entityを配置することのできるオブジェクトです。
* 階層的にサブレベルを配置することができます。
* Sceneのタイムスケールは子Sceneに継承されます。

## Entity
* Sceneの構成要素です。
* Entity自体は機能を持たせずComponentを追加していきます。

## Compoent
* Entityに追加する機能です。


## 使用例
### 構築
```c++
Ref<World> world = World::Create();

// シーン構築
Ref<Scene> scene = world->getRootScene();

// サブシーン追加
Ref<Scene> subScene = Scene::Create();
scene->addScene(scene);

// エンティティ生成とコンポーネント追加
auto entity = Entity::Create(scene);
if(auto mc = entity->addComponent<ModelComponent>()){
	mc->setModel(Model::Sphere());
}
scene->addEntity(entity);

// 子エンティティ生成
auto child = Entity::Create();
child->setParent(entity);

// エンティティ削除
child->requestRelease();

// WorldRendererを生成してGraphicに登録
auto renderer = WorldRenderer::Create();
renderer->setScene(scene);
```
### Component実装
```c++
class ModelComponent : public Component{
public:
	
	// RTTIに従うのであればこう
	ModelComponent(Entity& entity) : Component(entity) 
	{
		entity.addSceneEvent(
			[](const Ref<Scene>& scene){
				if(scene){
					scene->
				}
			}
		);
	}

	// 全てのコンストラクタで実装しなければいけなくなるのでinitialize()で初期化

	void setModel(const Ref<Model>& model)
	{
		m_model = model;
	}

private:
	void onActivate()
	{
		
	}
	void onDeactivate()
	{
		
	}

private:

	Ref<Model> m_model;
	
}
```
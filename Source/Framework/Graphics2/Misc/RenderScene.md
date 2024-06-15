RenderScene
-----------
RenderSceneは独立した1つの描画空間です。
エディタ画面ごとに作成され、お互いの描画内容は影響しません。

RenderSceneはRenderSystemから生成されます。RenderSystemはグローバルオブジェクトであるため、RenderScene::Create()を使用すると登録済みのRenderSystemからRenderSceneから構築されます。

```
RenderSystem
	RHI
	RenderScene[]
		RenderFeature[]
		RenderView[]
```





* RenderStage
    * Opaque/Masked/Transparentなど描画順
* CommandPriority
    * コマンド単位の描画順序
* SortOrder
    * カメラとの距離

コマンド単位で描画しないと並列描画の制約が生まれる


```
Map<RenderStage,Array<RenderItem>> itemMap;

addRenderItem(item);

for(auto& item:itemMap["Opaque"]){
	item
}
```


# RenderStage

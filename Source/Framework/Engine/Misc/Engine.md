Engine {#Engine}
================
Coreモジュールが特定のマネージャに依存しないクラスが集められているのに対して、Engineモジュールでは
ゲームループやモジュールの更新など、システムの基幹部分が実装されています。 

# 関数エントリ
Engineモジュールはプラットフォームごとのエントリ関数をラップし以下のエントリ関数を提供します。
* ```void OctbitInit(ob::ServiceInjector&);```
  * エンジン初期化前の処理
* ```void OctbitMain();```
  * Engine初期化後のメイン処理

## Engine
RuntimeとEditor両方から使用される機能です。
Engineクラスは内部にDIコンテナを持っており、入力システムやグラフィックシステムなどはこのコンテナに格納されています。各システムはEngineを経由してほかのシステムへアクセスします。
```cpp
auto system = Engine::GetService<SampleSystem>();
```

## Runtime
ランタイム起動のみで使用する機能です。


## Editor
エディタのみで使用する機能です。


## システムの登録
入力システムやグラフィックシステムなど全てのシステムはOptionalな機能として実装されます。各プロジェクトで必要な機能はOctbitInit関数内でDependencyGraphに登録してください。またシステムのコンフィグが存在する場合も同様にOctbitInit内で設定してください。
```cpp
void OctbitInit(ServiceInjector& injector){
	{
		input::Register(injector);
		rhi::Register(injector);
	}
	{
		InputConfig c;
		c.enableKeyboard = true;
		injector.bind(c);
	}
	{
		RHIConfig c;
		c.rhiPriorities = {
			"DirectX12",
			"Vulkan",
		};
		c.enablePIX = true;
		injector.bind(c);
	}
}
```

## サービス

### 生成単位
```
Engine
    Editor
        Tool[]
            World
    Game
        World
```
#### Engine
* シングルトンです。

```
Engine::GetService<T>();
```

#### Editor
* シングルトンです。  
* エディタ起動の場合のみ生成されます。

```
Editor::GetService<T>();
```

#### Game
* シングルトンです。
* ランタイム起動の場合のみ生成されます。
  * スタンドアロン起動
  * プレビュー
* ゲームプレビューとカットシーンプレビューは併用できません。

```
Game::GetService<T>();
```

#### World
* ワールドの数だけ生成されます
* ライティングや時間制御などが独立しています。
  
```
entity->getWorld()->getService<T>();
```

### エディタツールの扱い
* 複数アセットを同時編集する場合はツールごとにWorldが生成されます
  * ツール内で複数のWorldが生成される場合もあります
* エディタ起動の場合はGameのServiceは生成されません

# サブページ
* \subpage RTTI
* \subpage Scene
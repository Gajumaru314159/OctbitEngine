Engineモジュール
================
Coreモジュールが特定のマネージャに依存しないクラスが集められているのに対して、Engineモジュールでは
ゲームループやモジュールの更新など、システムの基幹部分が実装されています。 

# 関数エントリ
Engineモジュールはプラットフォームごとのエントリ関数をラップし以下のエントリ関数を提供します。
* ```void OctbitInit(ob::engine::EngineConfig&);```
  * エンジン初期化前の処理
* ```void OctbitMain();```
  * Engine初期化後のメイン処理

## GEngine
GEngineはアプリケーション内で唯一のEngineクラスのインスタンスです。Engineクラスは内部にDIコンテナを持っており、入力システムやグラフィックシステムなどはこのコンテナに格納されています。各システムはGEngineを経由してほかのシステムへアクセスします。
```cpp
auto system = GEngine->get<SampleSystem>();
```
## システムの登録
入力システムやグラフィックシステムなど全てのシステムはOptionalな機能として実装されます。各プロジェクトで必要な機能はOctbitInit関数内でDependencyGraphに登録してください。またシステムのコンフィグが存在する場合も同様にOctbitInit内で設定してください。
```c++
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
			TC("DirectX12"),
			TC("Vulkan"),
		};
		c.enablePIX = true;
		injector.bind(c);
	}
}
```
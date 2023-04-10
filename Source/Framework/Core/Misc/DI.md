DI (Dependency Injection)
============
## 概要
各サービス(システム)は依存するサービスをRequireかOptionalから選択して設定します。  
* Require：必須依存
* Optional：任意依存

サービスはインターフェイス化することもでき、複数の実装がある場合はプライオリティの高いサービスが選択されます。インターフェイスはRequire/Optionalどちらでも使用可能です。

```mermaid
graph LR
    Engine-->Graphic-->RHI-->|impl| DX12RHI & VulkanRHI
    DX12RHI-->Platform
    DX12RHI-.-> Profiler -->|impl| PIXProfiler & RenderDocProfiler
```
サービスの依存関係はコンストラクタの引数から判定されます。
|型|意味|
|-|-|
|T&|必須依存|
|T*|任意依存|

## 実装例

```cpp
class RHI{};
class DX12RHI:public RHI{   public: DX12RHI(Platform&,Profiler*); };
class Grahic{   public: Graphic(RHI&); };
class Engine{   public: Engine(Graphic&,Sound&);  };

int main(){

    ServiceInjector injector;
    // 中略
    injector.bind<DX12RHI>().as<RHI>();
    injector.bind<Graphic>();
    injector.bind<Engine>();
    
    ServiceContainer container;
    auto engine = injector.create<Engine>(container);
    
    // 最後に生成されたRHIを使用して生成
    Texture::White();


    return 0;
}
```
Engineがコンテナを持つ場合、依存定義用の内部クラスを登録/生成してください。  
```cpp
class Engine{
private:
    class EngineService{
        EngineService(Model&,Sound&){}
    };
public:

    Engine(ServiceInjector& injector){
        injector.bind<EngineService>();
        injector.create<EngineService>(m_container);
    }

    template<class T>
    T* getService()const{
        return m_container.get<T>();
    }

private:
    ServiceContainer m_container;
};

```

# ServiceInjectorへの登録
必須依存のサービスをServiceInjectorに登録する責務はそのサービスにあります。  
登録用の関数を使用すると内部的に必要なサービスが登録されます。
```cpp
void func(){
    ServiceInjector injector;
    ob::rhi::dx12::Register(injector);
    ob::graphics::Register(injector);

    auto graphics = injector.create<Graphics>();
}
//-----------------------------------------------------
void ob::graphics::Register(ServiceInjector& injector){
    injector.bind<Graphics>();
    injector.bind<EmptyRHI>().as<RHI>();
}
void ob::rhi::dx12::Register(ServiceInjector& injector){
    injector.bind<DX12RHI>().as<RHI>();
    injector.bind<Platform>();
}
```


# サービスの生成キャンセル
必須のサービスが生成されていない場合はコンストラクタから例外を送信することで生成をキャンセルすることができます。  
```cpp
struct Base{};
struct A:Base{
    A(SPtr<Req> req){
        if(!req){
            throw Exception();
        }
    }
};
struct B:Base{
    B(){

    }
};
struct C{
    C(SPtr<Base>);
}

void func(){
    ServiceInjector injector;
    injector.bind<Base>.to<A>();
    injector.bind<Base>.to<B>();
    injector.bind<C>.toSelf();
    injector.create<C>();
    // Cに渡されるのはB
}


```

# 生成単位
```
Engine
    Editor
        Tool[]
            World
    Game
        World
```
## Engine
* シングルトンです。

```
Engine::GetService<T>();
```

## Editor
* シングルトンです。  
* エディタ起動の場合のみ生成されます。

```
Editor::GetService<T>();
```

## Game
* シングルトンです。
* ランタイム起動の場合のみ生成されます。
  * スタンドアロン起動
  * プレビュー
* ゲームプレビューとカットシーンプレビューは併用できません。

```
GameInstance::GetService<T>();
```

## World
* ワールドの数だけ生成されます
  
```
entity->getWorld()->getService<T>();
```

# エディタツールの扱い
* 複数アセットを同時編集する場合はツールごとにWorldが生成されます
  * ツール内で複数のWorldが生成される場合もあります
* エディタ起動の場合はGameのサービスは生成されません
* 
DI (Dependency Injection)
============
各モジュールは依存するモジュールをRequireかOptionalから選択して設定します。  
* Require：必須依存
* Optional：任意依存

モジュールはインターフェイス化することもでき、複数の実装がある場合はプライオリティの高いモジュールが選択されます。インターフェイスはRequire/Optionalどちらでも使用可能です。

```mermaid
graph LR
    Graphic-->RHI & Engine

    RHI-->|impl| DX12RHI & VulkanRHI & MetalRHI
    
    DX12RHI-->Platform
    DX12RHI-.-> Profiler

    Profiler-->|impl| PIXProfiler & RenderDocProfiler
```

```mermaid
graph LR
    Calculator-->Operator-->|impl|Adder & Multiplier
    Calculator-.->Logger
```
モジュールの依存関係はDependencyGraphに登録します。DependencyGraphには構築可能なクラスと構築済みのインスタンスのみ登録できます。


```c++

class DX12RHI{
public:
    static void Register(DependencyGraph& graph){
        graph.add<DX12RHI>()
            .as<RHI>()
            .require<Engine>()
            .require<Platform>()
            .optional<Profiler>();
    }
public:
    DX12RHI(ServiceContainer& container);
}


class Engine{
public:

    void startup(){

        DependencyGraph graph;
        graph.add(*this);
        DX12RHI::Register(graph);
        
        m_container = {graph};
    }

private:
    ServiceContainer m_container;
}

```
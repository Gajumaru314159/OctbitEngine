DI
============

インスタンスの登録と依存性に合わせた生成

```


void ob::engine::AddServies(DISet& set){
    set.add<DX12RHIModule>().as<RHIModule>();
    set.add<VulkanRHIModule>().as<RHIModule>();
}

void rhi::DX12RHIModule::RegisterServie(DependencyGraph& graph){
    graph.add<DX12RHIModule>()
        .as<RHIModule>()
        .dependsOn<Engine>()
        .dependsOn<PlatformModule>()
        .dependsOn<Logger>();
}


class Engine{
public:

    void startup(){
        DependencyGraph graph;
        
        graph.add(*this);        
        rhi::DX12RHIModule::RegisterServie(graph);
        graphic::GraphicModule::RegisterServie(graph);

        graph.build();

        m_container = {graph};
    }

private:
    ServiceContainer m_container;
}

```
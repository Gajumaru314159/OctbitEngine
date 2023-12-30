描画コマンド


DrawInstancedIndirect
```cpp
struct DrawInstancedArgs {
  UINT VertexCountPerInstance;
  UINT InstanceCount;
  UINT StartVertexLocation;
  UINT StartInstanceLocation; // 0
};
```


# Phase 1
単純描画
```cpp
for(auto& objec:objects){
    if(object.visible == false) continue;
    for(auto& subset:object.subsets){
        cmd.draw(object.mesh,subset,object.material,"Opaque");
    }
}
```
# Phase 2
視錐台カリング
```cpp
Camera camera;
for(auto& objec:objects){
    if(object.visible == false) continue;
    if(camera.intersect(object.aabb)==false) contnue;
    for(auto& subset:object.subsets){
        cmd.draw(object.mesh,subset,object.material,"Opaque");
    }
}
```
# Phase 3
バッチ(インスタンス描画)
```cpp
Camera camera;
for(auto& objec:objects){
    if(object.visible == false) continue;
    if(camera.intersect(object.aabb)==false) contnue;
    for(auto& subset:object.subsets){
        auto hash = CalcHash(object.mesh,object.material);
        queues[hash].add(index);
    }
}
for(auto& queue:queues){
    auto& object = objects[queue.front()];
    
    Array<Matrix> matrixes;
    for(auto& index : queue){
        matrixes.push_back(objects[index].matrix);
    }

    cmd.drawInstanced(object.mesh,subset,object.material,"Opaque",matrixes);
}
```

# Phase 4
GPUカリング
```cpp
Camera camera;
for(auto& objec:objects){
    if(object.visible == false) continue;
    aabbs.push_back(object.aabb);
}

Buffer visibilities;
Buffer visibilityBuffer(aabbs);
GPUCulling(camera.buffer,aabbs,visibilities);

for(auto& queue:queues){
    auto& object = objects[queue.front()];
    
    Array<Matrix> matrixes;
    for(auto& index : queue){
        matrixes.push_back(objects[index].matrix);
    }

    cmd.drawInstancedIndirect(args);
}
```

# GPUバッファ
## AABB,Visibility,Matrix
* カリングせずに全てGPUに載せるか
    * Enableではないものに対して処理しない


# GPUCullingの条件
* Materialを使用している



# データ
```mermaid
flowchart LR
    RenderFeature-->RenderItem-->ICommand-->NativeCommand

```
* Commandはフレーム内バッファから生成される
* RenderItemはRF次第


# 描画順序

```cpp

for(auto& pipeline:m_pipelines){
    
    pipeline->render(recorder,ctx);

    for(auto& view:m_views){
        pipeline->render(recorder,view,ctx);
    }
}

void RenderPipeline::render(CommandRecorder& recorder,const RenderView& view,JobContext* ctx){
    
    

}



```









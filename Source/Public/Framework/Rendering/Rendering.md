Rendering
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


概要
----


```c++

model.draw(recorder);
//---------

struct MaterialTarget{
	TextureFormat colors[8];
	std::optional<TextureFormat> depth;
	InputLayout layout;
}



// 頂点レイアウトと書き込み先のフォーマットが必要
Material material(data,inputLayout);

material.record(recorder,model.getInputLayout());


```

# いつPipelineStateをつくるか


# 描画オブジェクト
MeshBuffer

```
class Mesh{
public:

}
```
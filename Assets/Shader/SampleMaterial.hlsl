#include <Material.hlsli>

struct MaterialParam
{
    float4          color;
    TextureHandle   albedo;
    SamplerHandle   albedoSampler;
};
struct SceneParam
{
    int dummy;
};
struct ViewParam
{
    int dummy;
};
struct DrawParam
{
    BufferHandle instances;
};


struct InstanceParam
{
    float4x4 world;
};


// IN / OUT
struct VsIn
{
    float4 pos      : POSITION;
    float4 normal   : NORMAL;
    float2 uv       : TEXCOORD;
    uint InstanceId : SV_InstanceID;  
};
struct PsIn
{
    float4 pos      : SV_POSITION;
    float4 normal   : NORMAL;
    float2 uv       : TEXCOORD;
    uint InstanceId : SV_InstanceID;  
};
struct PsOut
{
    float4 albedo   : SV_TARGET0; // Albedo Occlusion
};

// 
PsIn VS_Main(VsIn i)
{
    PsIn o;
    
    MaterialParam material = LoadBuffer(MaterialParam,MaterialHandle);
    SceneParam scene = LoadBuffer(SceneParam,SceneHandle);
    ViewParam view = LoadBuffer(ViewParam,ViewHandle);
    DrawParam draw = LoadBuffer(DrawParam,DrawHandle);
    
    InstanceParam instance = LoadBufferAt(InstanceParam,draw.instances,i.InstanceId);
    
    o.pos = mul(instance.world, i.pos);
    o.InstanceId = i.InstanceId;
    
    return o;
}
PsOut PS_Main(PsIn i)
{
    PsOut o;
       
    MaterialParam material = LoadBuffer(MaterialParam,MaterialHandle);
    SceneParam scene = LoadBuffer(SceneParam,SceneHandle);
    ViewParam view = LoadBuffer(ViewParam,ViewHandle);
    DrawParam draw = LoadBuffer(DrawParam,DrawHandle);
    
    InstanceParam instance = LoadBufferAt(InstanceParam,draw.instances,i.InstanceId);
    
    
    Texture2D<float4> albedo = LoadTexture(Texture2D,float4,material.albedo);
    SamplerState albedoSampler = LoadSampler(material.albedoSampler);
    
    o.albedo = albedo.Sample(albedoSampler, i.uv);
    
    
    return o;
}
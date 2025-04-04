#include "Bindless.hlsli"

ConstantBuffer<BufferHandle> MaterialHandle : register(b0,space0);

struct MaterialParam
{
    TextureHandle   albedo;
    SamplerHandle   albedoSampler;
    float4          color;
};

// IN / OUT
struct VsIn
{
    float4 pos      : POSITION;
    float2 uv       : TEXCOORD;
};
struct PsIn
{
    float4 pos      : SV_POSITION;
    float2 uv       : TEXCOORD;
};
struct PsOut
{
    float4 albedo   : SV_TARGET0; // Albedo Occlusion
};

// 
PsIn VS_Main(VsIn i)
{
    PsIn o;

    o.pos = i.pos;
    o.uv = i.uv;
    
    return o;
}
PsOut PS_Main(PsIn i)
{
    PsOut o;
       
    MaterialParam material = LoadBuffer(MaterialParam,MaterialHandle);    
    Texture2D<float4> albedo = LoadTexture(Texture2D,float4,material.albedo);
    SamplerState albedoSampler = LoadSampler(material.albedoSampler);
    
    o.albedo = albedo.Sample(albedoSampler, i.uv);
        
    return o;
}
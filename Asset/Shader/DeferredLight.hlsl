//#include "Common.h"

SamplerState g_mainSampler:register(s0);

// Global
cbuffer Param : register(b0) {
  //float    s_scalars[4];
  float4   s_colors[1];
  float4x4 s_matrices[1];
};

// Local
cbuffer Param : register(b1) { 
  float    g_scalars[8];
  float4   g_colors[8];
  float4x4 g_matrices[8];
};




Texture2D s_dummy : register(t0);
Texture2D g_textures[] : register(t1);
//Texture2D g_albedo : register(t1);
//Texture2D g_normal : register(t2);
//Texture2D g_depth : register(t3);

// IN / OUT
struct VsIn {
  float4 pos	:POSITION;
  float2 uv		:TEXCOORD;
};
struct PsIn {
  float4 pos	:SV_POSITION;
  float2 uv		:TEXCOORD;
};
struct PsOut {
  float4 color	:SV_TARGET0;
};
// ƒGƒ“ƒgƒŠ
PsIn VS_Main(VsIn i) {
    PsIn o;
    o.pos = i.pos;
    o.pos.w = 1.0;
    o.uv = i.uv;
    return o;
}
PsOut PS_Main(PsIn i){
    PsOut o;

    Texture2D g_albedo = g_textures[0];
    Texture2D g_normal = g_textures[1];
    Texture2D g_depth = g_textures[2];
    Texture2D g_uv = g_textures[3];

    float4 albedo = g_albedo.Sample(g_mainSampler,i.uv);
    float4 normal = g_normal.Sample(g_mainSampler,i.uv)*0.5+0.5;
    float4 depth = g_depth.Sample(g_mainSampler,i.uv) / 0.003;
    float4 uv = g_uv.Sample(g_mainSampler,i.uv);
    float4 l = lerp(normal,depth,step(i.uv.y,0.5));
    float4 r = lerp(albedo,uv,step(i.uv.y,0.5));
    o.color = albedo;//lerp(l,r,step(i.uv.x,0.5));
    return o;
}
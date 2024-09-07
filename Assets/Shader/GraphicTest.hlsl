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



Texture2D s_skyTex : register(t0);
Texture2D g_mainTex:register(t1);

// IN / OUT
struct VsIn {
  float4 pos	:POSITION;
  float4 normal	:NORMAL;
  float2 uv		:TEXCOORD;
};
struct PsIn {
  float4 pos	:SV_POSITION;
  float4 normal	:NORMAL;
  float2 uv		:TEXCOORD;
};
struct PsOut {
  float4 albedo	:SV_TARGET0; // Albedo Occlusion
  float4 normal	:SV_TARGET1; // Metalic Specular Roughness
  float4 uv	    :SV_TARGET2; // WorldNormal
};

// ƒGƒ“ƒgƒŠ
PsIn VS_Main(VsIn i) {
    PsIn o;
    o.pos = mul(s_matrices[0],mul(g_matrices[0],float4(i.pos.xyz,1)));
    o.uv = i.uv;
    o.normal = i.normal;
    return o;
}
PsOut PS_Main(PsIn i){
    PsOut o;
    o.albedo = g_mainTex.Sample(g_mainSampler,i.uv) * g_colors[0];
    if(o.albedo.a < 0.5) discard;

    o.normal = float4((i.normal.xyz*0.5+0.5),1.0);
    o.uv = float4(i.uv,0,1);
    return o;
}
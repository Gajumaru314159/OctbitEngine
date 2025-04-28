//#include "Common.h"

struct TextureHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct SamplerHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct BufferHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct MaterialProps { 
	TextureHandle MainTex;
	SamplerHandle MainSmp;
	TextureHandle NormalTex;
	SamplerHandle NormalSmp;
	TextureHandle DepthTex;
	SamplerHandle DepthSmp;
	TextureHandle UVTex;
	SamplerHandle UVSmp;
};
struct GlobalProps {
	float4x4 MatrixTest;
	float Time;
};
struct SceneProps {
	float4 LightDir;
};
struct ViewProps {
	float4x4 MatrixV;
	float4x4 MatrixP;
	float4x4 MatrixVP;
	float4x4 MatrixInvV;
	float4x4 MatrixInvP;
	float4x4 MatrixInvVP;
	float4 CameraPos;
	float4 ScreenSize;
	float4 CameraUp;
	float4 CameraRight;
	float4 CameraFront;
};

cbuffer RootConstants : register(b0) {
	BufferHandle MaterialHandle;
	BufferHandle GlobalHandle;
	BufferHandle SceneHandle;
	BufferHandle ViewHandle;
};


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
// エントリ
PsIn VS_Main(VsIn i) {
    PsIn o;
    o.pos = i.pos;
    o.pos.w = 1.0;
    o.uv = i.uv;
    return o;
}
PsOut PS_Main(PsIn i){
    PsOut o;

	
	MaterialProps mparam = ByteAddressBuffer(ResourceDescriptorHeap[MaterialHandle.index]).Load<MaterialProps>(0);
	
	Texture2D g_mainTex = ResourceDescriptorHeap[mparam.MainTex.index];
	SamplerState g_mainSmp = ResourceDescriptorHeap[mparam.MainSmp.index];
	
	Texture2D g_normalTex = ResourceDescriptorHeap[mparam.NormalTex.index];
	SamplerState g_normalSmp = ResourceDescriptorHeap[mparam.NormalSmp.index];
	
	Texture2D g_depthTex = ResourceDescriptorHeap[mparam.DepthTex.index];
	SamplerState g_depthSmp = ResourceDescriptorHeap[mparam.DepthSmp.index];
	
	Texture2D g_uvTex = ResourceDescriptorHeap[mparam.UVTex.index];
	SamplerState g_uvSmp = ResourceDescriptorHeap[mparam.UVSmp.index];


    float4 albedo = g_mainTex.Sample(g_mainSmp,i.uv);
    float4 normal = g_normalTex.Sample(g_normalSmp,i.uv)*0.5+0.5;
    float4 depth = g_depthTex.Sample(g_depthSmp,i.uv) / 0.003;
    float4 uv = g_uvTex.Sample(g_uvSmp,i.uv);
    float4 l = lerp(normal,depth,step(i.uv.y,0.5));
    float4 r = lerp(albedo,uv,step(i.uv.y,0.5));
    
    float factor = max(dot(float3(0,1,0),normal.xyz * 2 - 1),0) * 0.5 + 0.5;
    o.color = albedo * float4(factor,factor,factor,1);
    return o;
}
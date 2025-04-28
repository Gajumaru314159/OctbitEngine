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
	TextureHandle ParameterTex;
	SamplerHandle ParameterSmp;
	float4x4 Matrix;
	float4 Color;
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

	MaterialProps mparam = ByteAddressBuffer(ResourceDescriptorHeap[MaterialHandle.index]).Load<MaterialProps>(0);
	GlobalProps gparam = ByteAddressBuffer(ResourceDescriptorHeap[GlobalHandle.index]).Load<GlobalProps>(0);
	ViewProps vparam = ByteAddressBuffer(ResourceDescriptorHeap[ViewHandle.index]).Load<ViewProps>(0);

	float4 worldPos = mul(mparam.Matrix,float4(i.pos.xyz,1));
    o.pos = mul(gparam.MatrixTest,worldPos);
    o.uv = i.uv;
    o.normal = i.normal;
    return o;
}
PsOut PS_Main(PsIn i){

	MaterialProps mparam = ByteAddressBuffer(ResourceDescriptorHeap[MaterialHandle.index]).Load<MaterialProps>(0);
	GlobalProps gparam = ByteAddressBuffer(ResourceDescriptorHeap[GlobalHandle.index]).Load<GlobalProps>(0);
	
	Texture2D g_mainTex = ResourceDescriptorHeap[mparam.MainTex.index];
	SamplerState g_mainSmp = ResourceDescriptorHeap[mparam.MainSmp.index];
	
	Texture2D g_normalTex = ResourceDescriptorHeap[mparam.NormalTex.index];
	SamplerState g_normalSmp = ResourceDescriptorHeap[mparam.NormalSmp.index];
	
	Texture2D g_parameterTex = ResourceDescriptorHeap[mparam.ParameterTex.index];
	SamplerState g_parameterSmp = ResourceDescriptorHeap[mparam.ParameterSmp.index];

    PsOut o;
    o.albedo = g_mainTex.Sample(g_mainSmp,i.uv) * mparam.Color;
    if(o.albedo.a < 0.5) discard;

    //o.normal = float4((i.normal.xyz*0.5+0.5),1.0);
    o.normal = g_normalTex.Sample(g_normalSmp, i.uv)*0.5+0.5;
    o.uv = float4(i.uv,0,1);
    return o;
}
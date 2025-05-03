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
};
struct VsOut {
  float4 pos	:SV_POSITION;
};
struct PsOut {
  float4 color	:SV_TARGET0;
};

// ƒGƒ“ƒgƒŠ
VsOut VS_Main(VsIn i) {    
    VsOut o;

	MaterialProps mparam = ByteAddressBuffer(ResourceDescriptorHeap[MaterialHandle.index]).Load<MaterialProps>(0);
	GlobalProps gparam = ByteAddressBuffer(ResourceDescriptorHeap[GlobalHandle.index]).Load<GlobalProps>(0);
	ViewProps vparam = ByteAddressBuffer(ResourceDescriptorHeap[ViewHandle.index]).Load<ViewProps>(0);

	float4 worldPos = mul(mparam.Matrix,float4(i.pos.xyz,1));
    o.pos = mul(gparam.MatrixTest,worldPos);
    return o;
}

PsOut PS_Main(VsOut i) {    
    PsOut o;
	o.color = float4(0,0,0,0);
    return o;
}

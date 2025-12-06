//#include "Common.h"


struct TextureHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
	
	uint sampler_type;
	uint sampler_index;
	uint sampler_reserved0;
	uint sampler_reserved1;

	template<typename T>
	T Load() {
		return ResourceDescriptorHeap[index];
	}
	
	SamplerState LoadSampler() {
		return SamplerDescriptorHeap[sampler_index];
	}
};

struct BufferHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
	
	template<typename T>
	T Load() {
		return ByteAddressBuffer(ResourceDescriptorHeap[index]).Load<T>(0);
	}
};

struct MaterialProps { 
	TextureHandle Main;
	TextureHandle Normal;
	TextureHandle Parameter;
	float4x4 Matrix;
	float4 Color;
};
struct GlobalProps {
	float4x4 MatrixTest;
	float Time;
};
struct SceneProps {
	float4 LightDir;
	BufferHandle PointLight;
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

struct RootConstant {
	BufferHandle MaterialHandle;
	BufferHandle GlobalHandle;
	BufferHandle SceneHandle;
	BufferHandle ViewHandle;
};

#if defined(VULKAN)
#define ROOT_CONSTANT(type,name)	[[vk::push_constant]] type name
#elif defined(D3D)
#define	ROOT_CONSTANT(type,name)	ConstantBuffer<type> name : register(b0)
#endif

ROOT_CONSTANT(RootConstant,rc);


// IN / OUT
struct VsIn {
  float4 pos		:POSITION;
  float3 normal		:NORMAL;
  float3 tangent	:TANGENT;
  float2 uv			:TEXCOORD;
};
struct PsIn {
  float4 pos		:SV_POSITION;
  float3 normal		:NORMAL;
  float3 tangent	:TANGENT;
  float3 binormal	:BINORMAL;
  float2 uv			:TEXCOORD;
};
struct PsOut {
  float4 albedo	:SV_TARGET0; // Albedo Occlusion
  float4 normal	:SV_TARGET1; // Metalic Specular Roughness
};

// エントリ
PsIn VS_Main(VsIn i) {    
    PsIn o;

	BufferHandle MaterialHandle = rc.MaterialHandle;
	BufferHandle GlobalHandle = rc.GlobalHandle;
	BufferHandle SceneHandle = rc.SceneHandle;
	BufferHandle ViewHandle = rc.ViewHandle;

	MaterialProps mparam = ByteAddressBuffer(ResourceDescriptorHeap[MaterialHandle.index]).Load<MaterialProps>(0);
	GlobalProps gparam = ByteAddressBuffer(ResourceDescriptorHeap[GlobalHandle.index]).Load<GlobalProps>(0);
	ViewProps vparam = ByteAddressBuffer(ResourceDescriptorHeap[ViewHandle.index]).Load<ViewProps>(0);

	float4 worldPos = mul(mparam.Matrix,float4(i.pos.xyz,1));
    o.pos = mul(gparam.MatrixTest,worldPos);
    o.uv = i.uv;
    o.normal = normalize(mul(mparam.Matrix,float4(i.normal,0.0))).xyz;
    o.tangent = normalize(mul(mparam.Matrix,float4(i.tangent,0.0))).xyz;
    o.binormal = cross(o.normal,o.tangent);
    return o;
}
PsOut PS_Main(PsIn i){
	BufferHandle MaterialHandle = rc.MaterialHandle;
	BufferHandle GlobalHandle = rc.GlobalHandle;
	BufferHandle SceneHandle = rc.SceneHandle;
	BufferHandle ViewHandle = rc.ViewHandle;

	MaterialProps mparam = MaterialHandle.Load<MaterialProps>();
	GlobalProps gparam = GlobalHandle.Load<GlobalProps>();
	
	Texture2D g_mainTex = mparam.Main.Load<Texture2D>();
	SamplerState g_mainSmp = mparam.Main.LoadSampler();
	
	Texture2D g_normalTex = mparam.Normal.Load<Texture2D>();
	SamplerState g_normalSmp = mparam.Normal.LoadSampler();
	
	Texture2D g_parameterTex = mparam.Parameter.Load<Texture2D>();
	SamplerState g_parameterSmp = mparam.Parameter.LoadSampler();

    PsOut o;
    o.albedo = g_mainTex.Sample(g_mainSmp,i.uv) * mparam.Color;
    if(o.albedo.a < 0.5) discard;

	float3 tn = g_normalTex.Sample(g_normalSmp, i.uv).xyz * 2.0 - 1.0;
	float3 n = tn.x * i.binormal + tn.y * i.tangent + tn.z * i.normal;
	o.normal = float4(normalize(n) * 0.5 + 0.5,1.0);

    return o;
}
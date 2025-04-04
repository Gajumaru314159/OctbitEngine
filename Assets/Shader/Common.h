
SamplerState g_mainSampler:register(s0);



struct StructuredParam {
	float4   s_colors[1];
	float4x4 s_matrices[1];
};;
StructuredBuffer<StructuredParam > params : register(t0, space1);

// s Sampler
// t SRV (Texture/StructuredBuffer/ByteAddressBuffer)
// u UAV (Texture/Buffer) 順序指定なしのRW用
// b CBV (ConstantBuffer)


// space0 : システム用

struct MaterialParam {
	int material;
	int instance;
};
cbuffer RootConstant : register(b0, space0) {
	MaterialParam param;
};

struct InstanceParam {
	float4x4 world;
};
StructuredParam<InstanceParam> instances : register(t0, space0);


// Global (パラメータ数はシェーダ次第)
cbuffer Param : register(b0) {
	float    s_scalars[1];
	float4   s_colors[1];
	float4x4 s_matrices[1];
};

// Local (パラメータ数はシェーダ次第)
cbuffer Param : register(b1) {
	float    g_scalars[8];
	float4   g_colors[8];
	float4x4 g_matrices[8];
#if BINDLESS_ENABLED
	int g_buffer[8];
	int g_texture[8];
#endif
};

// space9 : バッファパラメータ
struct LightParam2 {
	float4   s_lightColors[1];
	float4x4 s_lightMatrices[1];
};
cbuffer LightParam : register(b0, space9) {
	LightParam2 g_lightParam;
};

StructuredBuffer<LightParam2> params : register(t0, space1);

ByteAddressBuffer params : register(t0, space1);


// space10 : テクスチャパラメータ
Texture2D<float>  g_texture0[0] : register(t0, space10);
Texture2D<float2>  g_texture0[0] : register(t0, space11);
Texture2D<float3>  g_texture0[0] : register(t0, space12);
Texture2D<float4>  g_texture0[0] : register(t0, space13);


RWTexture2D<float>  g_texture3 : register(u3, space10);
RWTexture2D<float>  g_texture4 : register(u4, space10);


VS_OUTPUT VS_Main(VS_INPUT input) {
	VS_OUTPUT output;
	float4 worldPos = mul(input.position,instances[param.instance]);
	output.position = mul(worldPos, g_viewProjection);
	return output;
}
PS_OUTPUTT PS_Main(PS_INPUT input) {
	instances[param.instance];
}
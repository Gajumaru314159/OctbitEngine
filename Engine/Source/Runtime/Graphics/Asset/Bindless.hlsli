#include "Config.hlsli"

#if BINDLESS_ENABLED

//==============================================================================
// バインドレスハンドル
//==============================================================================

struct TextureHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
	
	template<typename T>
	T Load() {
		return ResourceDescriptorHeap[index];
	}
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
	
	template<typename T>
	T Load() {
		return ByteAddressBuffer(ResourceDescriptorHeap[index]).Load<T>(0);
	}
};



//==============================================================================
// Load
//==============================================================================

#if BINDLESS_V1_ENABLED

#define DEFINE_TEXTURE_TABLE(OBJECT, TYPE, REGISTER, SPACE) OBJECT<TYPE> ResourceDescriptorHeap_##OBJECT##_##TYPE[] : register(REGISTER, SPACE)

ByteAddressBuffer SamplerDescriptorHeap[] : register(s0,space20);
ByteAddressBuffer ResourceDescriptorHeap_ByteAddressBuffer[] : register(u0,space20);
DEFINE_TEXTURE_TABLE(Texture2D      ,float ,t0,space21);    // Height
DEFINE_TEXTURE_TABLE(Texture2D      ,float2,t0,space22);    // Normal/Velocity/Flow
DEFINE_TEXTURE_TABLE(Texture2D      ,float4,t0,space23);    // Albedo
DEFINE_TEXTURE_TABLE(Texture2D      ,uint  ,t0,space24);    // Id
DEFINE_TEXTURE_TABLE(Texture3D      ,float ,t0,space25);    // Volume
DEFINE_TEXTURE_TABLE(Texture3D      ,float4,t0,space26);    // Volume
DEFINE_TEXTURE_TABLE(TextureCube    ,float4,t0,space27);    // ReflectionCube
DEFINE_TEXTURE_TABLE(RWTexture2D    ,float ,u0,space28);    
DEFINE_TEXTURE_TABLE(RWTexture2D    ,float2,u0,space29);    
DEFINE_TEXTURE_TABLE(RWTexture2D    ,float4,u0,space30);
DEFINE_TEXTURE_TABLE(RWTexture2D    ,uint  ,u0,space31);
DEFINE_TEXTURE_TABLE(RWTexture3D    ,float ,u0,space32);
DEFINE_TEXTURE_TABLE(RWTexture3D    ,float4,u0,space33);

#define LoadTexture(OBJECT,TYPE,HANDLE) (ResourceDescriptorHeap_##OBJECT##_##TYPE[HANDLE.index])
#define LoadSampler(HANDLE) (SamplerDescriptorHeap[HANDLE.index])
#define LoadBuffer(TYPE,HANDLE) (ResourceDescriptorHeap_ByteAddressBuffer[HANDLE.index].Load<TYPE>())
#define LoadBufferAt(TYPE,HANDLE,INDEX) (ResourceDescriptorHeap_ByteAddressBuffer[HANDLE.index].Load<TYPE>(sizeof(TYPE)*INDEX))

#endif // BINDLESS_V1_ENABLED
#if BINDLESS_V2_ENABLED

ByteAddressBuffer LoadBufferImpl(BufferHandle handle) {
    return ResourceDescriptorHeap[handle.index];
}

template<typename T>
StructuredBuffer<T> LoadBufferImpl2(BufferHandle handle)
{
    return ResourceDescriptorHeap[handle.index];
}

#define LoadTexture(OBJECT,TYPE,HANDLE) (OBJECT<TYPE>(ResourceDescriptorHeap[HANDLE.index]))
#define LoadSampler(HANDLE) (SamplerDescriptorHeap[HANDLE.index])
#define LoadBuffer(TYPE,HANDLE) (ByteAddressBuffer(ResourceDescriptorHeap[HANDLE.index]).Load<TYPE>(0))
#define LoadBufferAt(TYPE,HANDLE,INDEX) (StructuredBuffer<TYPE>(ResourceDescriptorHeap[HANDLE.index])[INDEX])

#endif // BINDLESS_V2_ENABLED

#endif // BINDLESS_ENABLED
#include "Bindless.hlsli"

ConstantBuffer<BufferHandle> MaterialHandle : register(b0,space0);
ConstantBuffer<BufferHandle> SceneHandle : register(b1,space0);
ConstantBuffer<BufferHandle> ViewHandle : register(b2,space0);
ConstantBuffer<BufferHandle> DrawHandle : register(b3,space0);

// float,float2,float3,float4,uint,uint4
// Texture2D,Texture2DArray,Texture3D,TextureCube


Texture2D<float> g_BindlessTextures_Float[] : register(t0, space1);
Texture2D<float> g_BindlessTextures_Float[] : register(t0, space2);


Buffer s_BindlessBuffer[] : register(t0, space13);



// ShaderModel 6.6～
// タイプごとに分けずにResourceDescriptorHeapでアクセスできるようになる
// Texture2D<float3> texture = ResourceDescriptorHeap[index];
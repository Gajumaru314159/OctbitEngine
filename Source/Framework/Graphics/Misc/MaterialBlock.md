# MaterialBlock {#MaterialBlock}
Materialのプロパティ設定部分のみを切り出したクラスです。
グローバル用やアセット用、ビュー用など、共有される単位で分けて管理します。

## 生成方法
プロパティ名のリストを設定して生成します。
Bindfullモデルの場合はテクスチャ数やバッファ数が一致するDescriptorLayoutを追加で設定します。
Bindlessモデルの場合はPushConstantsでバインドを行うためDescriptorLayoutは必要ありません。もしBindfull用のDescriptorLayoutが設定されていたとしても無視されます。
```cpp
// Texture&Sampler、Buffer、パラメータ用のバッファの順に定義する
// ただしTexture
Ref<DescriptorLayout> layout = DescriptorLayout::Create({
	Binding::Texture(0),			// Albedo (Texture)
	Binding::Sampler(1),			// Albedo (Sampler)
	Binding::Texture(2),			// Normal (Texture)
	Binding::Sampler(3),			// Normal (Sampler)
	Binding::ByteAddressBuffer(4),	// Buffer
	Binding::ByteAddressBuffer(5),	// パラメーター(Color~Height)
});

MaterialBlockDesc desc;
desc.name = "TestBlock";
desc.layout = layout;
desc.textures = { "Albedo" ,"Normal" };
desc.buffers = { "Instances" };
desc.vectors = { "Color" };
desc.scalars = { "Progress","Speed", "Time", "Width", "Height" };
```
## プロパティの設定
プロパティタイプごとにセッターが用意されています。
TextureプロパティはTextureとSamplerをセットする必要があります。
```cpp
block.setTexture("Texture", Texture::Check(),Sampler::Default());
block.setScalar("Speed", 3.f);
block.setScalar("Width", 1280.f);
block.setScalar("Height", 720.f);
block.setVector("Color", Color::Cyan);
```

## CommandListへの記録
```MaterialBlock::record()```を使用します。引数には記録したいCommandListと、RootSignatureの何番目のスロットに灯篭くするかを指定します。
```cpp
s32 slot = 0;
block.record(commandList, slot);
```

## シェーダー側の設定
シェーダー側ではMaterialDescに設定したパラメーターの順にプロパティが設定されます。

### Bindfull
```hlsl
Texture2D		Albedo			: register(t0);
SamplerState	AlbedoSampler	: register(s1);

Texture2D		Normal			: register(t2);
SamplerState	NormalSampler	: register(s3);

ByteAddressBuffer Instances		: register(t4);

cbuffer Params : register(t5) {
	float4 Color;
	float Progress;
	float Speed;
	float Time;
	float Width;
	float Height;
}
```

### Bindless
Bindlessモデルの場合はパラメーターバッファの先頭にTexture、Sampler、BufferのBindlessHandleが格納されます。
コードの見やすさのためにタイプごとのBindlessHandleのエイリアス(TextureHandle,SamplerHandle,BufferHandle)を使用してください。
パラメーターバッファ自体のBindlessHandleはPushConstantsで渡されます。
```hlsl
struct Params {
	TextureHandle AlbedoHandle;
	SamplerHandle AlbedoSamplerHandle;
	TextureHandle NormalHandle;
	SamplerHandle NormalSamplerHandle;
	BufferHandle InstancesHandle;
	float4 Color;
	float Progress;
	float Speed;
	float Time;
	float Width;
	float Height;
};

ConstantBuffer<BufferHandle> ParamsHandle : register(b0);

PsOut PsMain(PsIn i) {
	PsOut o;

	Params params = ParamsHandle.Load<Params>();
	Texture2D albedo = params.albedoHandle.Load<Texture2D>();

	return o;
}

```

## 複数のMaterialBlockの使用
Bindfullモデルの場合はスロット毎に```record()```を呼び出してください。
Bindlessモデルの場合はrecordの第二引数にPushConstans時のバイトオフセットを指定してください。
```cpp
// Bindfull
s32 slot = 0;
block1.record(commandList, slot);

// Bindless
block2.record(commandList, slot, sizeof(BindlessHandle) * 1);
```

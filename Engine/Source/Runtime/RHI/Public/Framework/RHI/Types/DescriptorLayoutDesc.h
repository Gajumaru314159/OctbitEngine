//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

	//! @brief バインディングタイプ
	//! 
	//! @details バインディングタイプは各グラフィックAPIでは以下の表の対応関係にあります。
	//! | BindingType			| HLSL						| D3D12				| GLSL				| Vulkan				|
	//! |-----------------------|---------------------------|-------------------|-------------------|-----------------------|
	//! | Texture				| (t) Texture2D				| SRV				| sampler2D			| eSampledImage			|
	//! | RWTexture				| (u) RWTexture2D			| UAV				| image2D			| eStorageImage			|
	//! | Buffer				| (t) Buffer<T>				| SRV				| buffer			| eUniformTexelBuffer	|
	//! | RWBuffer				| (u) RWBuffer<T>			| UAV				| buffer			| eStorageTexelBuffer	|
	//! | StructuredBuffer		| (t) StructuredBuffer<T>	| SRV				| buffer			| eStorageBuffer		|
	//! | RWStructuredBuffer	| (u) RWStructuredBuffer<T>	| UAV				| buffer			| eStorageBuffer		|
	//! | ByteAddressBuffer		| (t) ByteAddressBuffer		| SRV				| buffer			| eStorageBuffer		|
	//! | RWByteAddressBuffer	| (u) RWByteAddressBuffer	| UAV				| buffer			| eStorageBuffer		|
	//! | ConstantBuffer		| (b) ConstantBuffer		| CBV				| uniform			| eUniformBuffer		|
	//! | Sampler				| (s) SamplerState			| Sampler			| sampler			| eSampler				|
	enum class BindingType : u32 {
		Unknown = 0xFFFFFFFF,
		Texture = 0,
		RWTexture,
		Buffer,
		RWBuffer,
		StructuredBuffer,
		RWStructuredBuffer,
		ByteAddressBuffer,
		RWByteAddressBuffer,
		ConstantBuffer,
		Sampler,
	};

	struct BindingItem {
		BindingType type;
		s32         index; // register / binding

		constexpr BindingItem(BindingType type, s32 index)
			: type(type), index(index) {}
	};

	//! @brief      BindingItem定義のユーティリティ
	struct Binding {

		#define DECL_BINDING(TYPE) \
		static constexpr BindingItem TYPE(s32 index) {\
			return BindingItem(BindingType::TYPE, index);\
		}

		// indexが負の場合は前のBindingItemのindexからの相対値を表します。
		// 先頭のBindingItemが負の数の場合は0に置き換えられます
		DECL_BINDING(Texture);
		DECL_BINDING(RWTexture);
		DECL_BINDING(Buffer);
		DECL_BINDING(RWBuffer);
		DECL_BINDING(StructuredBuffer);
		DECL_BINDING(RWStructuredBuffer);
		DECL_BINDING(ByteAddressBuffer);
		DECL_BINDING(RWByteAddressBuffer);
		DECL_BINDING(ConstantBuffer);
		DECL_BINDING(Sampler);

		#undef DECL_BINDING
	};

	struct DescriptorLayoutDesc {
		String				name;		//!< 名前
		Vector<BindingItem> items;		//!< バインディングスロット

		DescriptorLayoutDesc() = default;
		DescriptorLayoutDesc(std::initializer_list<BindingItem> items) : items(items) {}
	};

}
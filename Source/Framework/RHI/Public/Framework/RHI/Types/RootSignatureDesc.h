//***********************************************************
//! @file
//! @brief		ルートシグネチャ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Types/ShaderStage.h>
#include <Framework/RHI/Types/SamplerDesc.h>

namespace ob::rhi {

#pragma region Enum


	enum class BindingType {
		// [HLSL]						[D3D12]				[GLSL]						[Vulkan]
		Texture,				// (t) Texture2D				SRV					sampler2D					eSampledImage
		RWTexture,				// (u) RWTexture2D				UAV					image2D						eStorageImage
		Buffer,					// (t) Buffer<T>				SRV					buffer						eUniformTexelBuffer
		RWBuffer,				// (u) RWBuffer<T>				UAV					buffer						eStorageTexelBuffer
		StructuredBuffer,		// (t) StructuredBuffer<T>		SRV					buffer						eStorageBuffer
		RWStructuredBuffer,		// (u) RWStructuredBuffer<T>	UAV					buffer						eStorageBuffer
		ByteAddressBuffer,		// (t) ByteAddressBuffer		SRV					buffer						eStorageBuffer
		RWByteAddressBuffer,	// (u) RWByteAddressBuffer		UAV					buffer						eStorageBuffer
		ConstantBuffer,			// (b) ConstantBuffer			CBV					uniform						eUniformBuffer
		Sampler,				// (s) SamplerState				Sampler				sampler						eSampler
	};


#pragma endregion

#pragma region Flag

	//! @brief      ルートシグネチャ・フラグ
	//! @see        RootSignatureDesc
	enum class RootSignatureFlag :u32 {
		None							= 0,
		AllowInputAssemblerInputLayout  = get_bit(0),	//!< 頂点入力あり
		DenyVertexShaderAccess			= get_bit(1),	//!< 
		DenyHullShaderAccess			= get_bit(2),	//!< 
		DenyDomainShaderAccess			= get_bit(3),	//!< 
		DenyGeometryShaderAccess		= get_bit(4),	//!< 
		DenyPixelShaderAccess			= get_bit(5),	//!< 
		AllowStreamOutput				= get_bit(6),	//!< 
		DenyAmplificationShaderAccess	= get_bit(7),	//!< 
		DenyMeshShaderAccess			= get_bit(8),	//!< 
		EnableBindless					= get_bit(9),	//!< 
	};
	//! @brief      ルートシグネチャ・フラグ・セット
	//! @see        RootSignatureDesc
	using RootSignatureFlags = BitFlags<RootSignatureFlag>;

#pragma endregion

#pragma region Sub Structure

	//! @brief      ルートコンスタント定義
	//! @see        RootParameter
	struct RootConstantsDesc {
		u32 size;          //!< 値
		u32 registerNo;     //!< レジスタ番号 (D3D12のみ)
		u32 registerSpace;  //!< レジスタ空間 (D3D12のみ)
	public:
		//! @brief      コンストラクタ
		RootConstantsDesc() : RootConstantsDesc(0, 0, 0) {}
		//! @brief      コンストラクタ(DescriptorTable)
		RootConstantsDesc(u32 size,u32 registerNo, u32 registerSpace=0)
			:size(size),registerNo(registerNo), registerSpace(registerSpace) {}


		RootConstantsDesc& set(u32 size, u32 registerNo, u32 registerSpace = 0) {
			this->size = size;
			this->registerNo = registerNo;
			this->registerSpace = registerSpace;
			return *this;
		}
	};

	//! @brief      スタティックサンプラー定義
	//! 
	//! @details    レジスタ番号とレジスタ空間はシェーダで定義された値と一致させる必要があります。
	//!             レジスタ番号が1、レジスタ空間が0の場合はHLSLでは以下のように定義されます。
	//!             ```SamplerState  mainSampler : register(s1,space0)```
	//!             レジスタ空間を分けることで、同じレジスタ番号を使用することができます。
	//! @see        RootSignatureDesc
	struct StaticSamplerDesc {
		SamplerDesc     sampler;        //!< サンプラー設定
		u32             registerNo;     //!< レジスタ番号
		u32             registerSpace;  //!< レジスタ空間
		ShaderStage		visibility;		//!< どのシェーダステージから利用可能か
	public:
		//! @brief      コンストラクタ
		StaticSamplerDesc() = default;

		//! @brief      コンストラクタ
		StaticSamplerDesc(const SamplerDesc& sampler, u32 registerNo, u32 registerSpace=0, ShaderStage visibility = ShaderStage::All)
			:visibility(visibility), sampler(sampler), registerNo(registerNo), registerSpace(registerSpace) {}
	};

#pragma endregion

	//! @brief      静的サンプラー・配列
	using StaticSamplerArray = FixedVector<StaticSamplerDesc, STATIC_SAMPLER_MAX>;



	struct BindingItem {
		BindingType type;
		s32         index; // register / binding 負の数の場合は前の要素からの相対値
		s32         space; // space / set

		constexpr BindingItem(BindingType type, s32 index, s32 space)
			: type(type), index(index), space(space) {}
	};

	struct BindingSlot {
		Vector<BindingItem> items;

		BindingSlot() = default;
		BindingSlot(std::initializer_list<BindingItem> items) : items(items) {}
	};

	struct BindingLayoutDesc {
		String				name;		//!< 名前
		Vector<BindingSlot> slots;		//!< バインディングスロット
		StaticSamplerArray	samplers;	//!< 静的サンプラー
		RootConstantsDesc	constants;	//!< ルートコンスタント
		RootSignatureFlags	flags;		//!< フラグ

		BindingLayoutDesc() = default;
		BindingLayoutDesc(Vector<BindingSlot> slots) : slots(slots) {}
		BindingLayoutDesc(std::initializer_list<BindingSlot> slots) : slots(slots) {}
	};

	//! @brief      BindingItem定義のユーティリティ
	struct Binding {

#define DECL_BINDING(TYPE) \
		static constexpr BindingItem TYPE(s32 index = -1, s32 space = 0) {\
			return BindingItem(BindingType::TYPE, index, space);\
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

}
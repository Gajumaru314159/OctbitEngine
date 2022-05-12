//***********************************************************
//! @file
//! @brief		ルートシグネチャ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/ShaderStage.h>
#include <Framework/Graphic/Types/SamplerDesc.h>

namespace ob::graphic {

#pragma region Enum

	//@―---------------------------------------------------------------------------
	//! @brief      ルートシグネチャスロットのタイプ
	//! 
	//! @note       RootConstants以外はDescriptorTableで代替え可能
	//! @see        RootParameter
	//@―---------------------------------------------------------------------------
	enum class RootParameterType :u32 {
		CBV,                //!< 定数バッファ・ビュー
		SRV,                //!< シェーダ・リソース・ビュー
		UAV,                //!< UnorderedAccessView
		DescriptorTable,    //!< デスクリプタ・テーブル
		RootConstants,      //!< ルート定数
	};


	//@―---------------------------------------------------------------------------
	//! @brief      デスクリプタ範囲タイプ
	//! @see        RootDescriptorTable
	//@―---------------------------------------------------------------------------
	enum class DescriptorRangeType :u32 {
		CBV,                //!< 定数バッファ・ビュー
		SRV,                //!< シェーダ・リソース・ビュー
		UAV,                //!< UnorderedAccessView
		Sampler,            //!< サンプラー
	};

#pragma endregion

#pragma region Flag

	//@―---------------------------------------------------------------------------
	//! @brief      ルートシグネチャ・フラグ
	//! @see        RootSignatureDesc
	//@―---------------------------------------------------------------------------
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
	};
	//@―---------------------------------------------------------------------------
	//! @brief      ルートシグネチャ・フラグ・セット
	//! @see        RootSignatureDesc
	//@―---------------------------------------------------------------------------
	using RootSignatureFlags = bit_flags<RootSignatureFlag>;

#pragma endregion

#pragma region Sub Structure

	//@―---------------------------------------------------------------------------
	//! @brief      デスクリプタ範囲
	//! @see        RootDescriptorTable
	//@―---------------------------------------------------------------------------
	struct DescriptorRange {
		DescriptorRangeType type;           //!< タイプ
		u32                 num;            //!< デスクリプタの数
		u32                 baseRegister;   //!< 開始レジスタ番号
		u32                 registerSpace;  //!< レジスタ空間
	public:
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		DescriptorRange() = default;
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(DescriptorTable)
		//@―---------------------------------------------------------------------------
		DescriptorRange(DescriptorRangeType type,u32 num, u32 baseRegister, u32 registerSpace = 0)
			:type(type), num(num),baseRegister(baseRegister), registerSpace(registerSpace) {}
	};


	//@―---------------------------------------------------------------------------
	//! @brief      ルートコンスタント定義
	//! @see        RootParameter
	//@―---------------------------------------------------------------------------
	struct RootConstantsDesc {
		u32 registerNo;     //!< レジスタ番号
		u32 registerSpace;  //!< レジスタ空間
		u32 value;          //!< 値
	public:
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RootConstantsDesc() = default;
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(DescriptorTable)
		//@―---------------------------------------------------------------------------
		RootConstantsDesc(u32 value,u32 registerNo, u32 registerSpace=0)
			:value(value),registerNo(registerNo), registerSpace(registerSpace) {}
	};


	//@―---------------------------------------------------------------------------
	//! @brief      ルートデスクリプタ定義
	//! 
	//! @details    1度にバインドする要素が1つの場合に使用可能です。
	//! @see        RootParameter
	//@―---------------------------------------------------------------------------
	struct RootDescriptorDesc {
		u32 registerNo;     //!< レジスタ番号
		u32 registerSpace;  //!< レジスタ空間
	public:
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RootDescriptorDesc() = default;
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(DescriptorTable)
		//@―---------------------------------------------------------------------------
		RootDescriptorDesc(u32 registerNo, u32 registerSpace=0)
			:registerNo(registerNo),registerSpace(registerSpace){}
	};

	//@―---------------------------------------------------------------------------
	//! @brief      ルートパラメータ
	//! @see        RootSignatureDesc
	//@―---------------------------------------------------------------------------
	struct RootParameter {
		RootParameterType		type;			//!< パラメータ・タイプ
		vector<DescriptorRange> descriptors;    //!< typeがDescriptorTableの場合使用
		RootConstantsDesc		constants;		//!< typeがRootConstantsの場合使用
		RootDescriptorDesc		descriptor;     //!< typeがCBV/SRV/UAVの場合使用
		ShaderStage				visibility;		//!< どのシェーダステージから利用可能か
	public:
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RootParameter() = default;
		//@―---------------------------------------------------------------------------
		//! @brief      デストラクタ
		//@―---------------------------------------------------------------------------
		~RootParameter() {}

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(DescriptorTable)
		//@―---------------------------------------------------------------------------
		RootParameter(vector<DescriptorRange>&& descriptorTable, ShaderStage visibility = ShaderStage::All)
			:type(RootParameterType::DescriptorTable), descriptors(descriptorTable), visibility(visibility) {}

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(RootConstants)
		//@―---------------------------------------------------------------------------
		RootParameter(RootConstantsDesc constants, ShaderStage visibility = ShaderStage::All)
			:type(RootParameterType::RootConstants), constants(constants), visibility(visibility) {}

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(CBV/SRV/UAV)
		//@―---------------------------------------------------------------------------
		RootParameter(RootParameterType type, RootDescriptorDesc descriptor, ShaderStage visibility = ShaderStage::All)
			:type(type), descriptor(descriptor), visibility(visibility) {}
	};


	//@―---------------------------------------------------------------------------
	//! @brief      スタティックサンプラー定義
	//! 
	//! @details    レジスタ番号とレジスタ空間はシェーダで定義された値と一致させる必要があります。
	//!             レジスタ番号が1、レジスタ空間が0の場合はHLSLでは以下のように定義されます。
	//!             ```SamplerState  mainSampler : register(s1,space0)```
	//!             レジスタ空間を分けることで、同じレジスタ番号を使用することができます。
	//! @see        RootSignatureDesc
	//@―---------------------------------------------------------------------------
	struct StaticSamplerDesc {
		SamplerDesc     sampler;        //!< サンプラー設定
		u32             registerNo;     //!< レジスタ番号
		u32             registerSpace;  //!< レジスタ空間
		ShaderStage		visibility;		//!< どのシェーダステージから利用可能か
	public:
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		StaticSamplerDesc() = default;

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		StaticSamplerDesc(const SamplerDesc& sampler, u32 registerNo, u32 registerSpace=0, ShaderStage visibility = ShaderStage::All)
			:visibility(visibility), sampler(sampler), registerNo(registerNo), registerSpace(registerSpace) {}
	};

#pragma endregion

	//@―---------------------------------------------------------------------------
	//! @brief      ルートシグネチャ定義
	//@―---------------------------------------------------------------------------
	struct RootSignatureDesc {
		vector<RootParameter>       parameters;	//!< ルートパラメータ
		vector<StaticSamplerDesc>   samplers;	//!< 固定サンプラー
		RootSignatureFlags          flags;		//!< フラグ
	public:
		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RootSignatureDesc() = default;

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RootSignatureDesc(vector<RootParameter>&& parameters, vector<StaticSamplerDesc>&& samplers, RootSignatureFlags flags = RootSignatureFlag::AllowInputAssemblerInputLayout)
			: parameters(parameters), samplers(samplers), flags(flags) {}
	};

}// namespcae ob
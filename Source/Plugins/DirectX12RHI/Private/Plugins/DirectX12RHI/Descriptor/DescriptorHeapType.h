//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
namespace ob::rhi {

	//! @brief  デスクリプタ・タイプ
	enum class DescriptorHeapType :u32 {
		CBV_SRV_UAV,    //!< 定数バッファ/シェーダリソース/アンオーダードアクセス
		RTV,            //!< レンダーターゲット
		DSV,            //!< デプスステンシル
		Sampler,        //!< サンプラー
		SamplerCopyable,//!< コピー可能サンプラー
	};

}
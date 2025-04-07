//***********************************************************
//! @file
//! @brief		グラフィック定数
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

	static constexpr s32 BACKBUFFER_MAX			= 3;		//!< バックバッファの最大数
	static constexpr s32 CBV_REGISTER_MAX		= 16;		//!< シェーダで利用できるコンスタント・バッファ・レジストリの最大数
	static constexpr s32 SRV_REGISTER_MAX		= 128;		//!< シェーダで利用できるシェーダ・リソース・レジストリの最大数
	static constexpr s32 UAV_REGISTER_MAX		= 16;		//!< シェーダで利用できるアンオーダー・アクセス・レジストリの最大数
	static constexpr s32 SAMPLER_REGISTER_MAX	= 16;		//!< シェーダで利用できるサンプラー・ステート・レジストリの最大数

	static constexpr s32 RENDER_TARGET_MAX		= 8;		//!< RenderTexture で使用できる描画先の最大数
	static constexpr s32 VIEWPORT_MAX			= 16;		//!< ビューポート最大数
	static constexpr s32 SCISSOR_RECT_MAX		= 4;		//!< シザー矩形最大数
	static constexpr s32 MIPMAP_MAX				= 16;		//!< ミップマップ最大数
	static constexpr s32 VERTEX_BUFFER_MAX		= 16;		//!< 頂点バッファのストーリム最大数

	static constexpr s32 VERTEX_ATTRIBUTE_MAX	= 16;		//!< 頂点属性の最大数
	static constexpr s32 ROOT_PARAMETER_MAX		= 16;		//!< ルートシグネチャのパラメータ最大数
	static constexpr s32 STATIC_SAMPLER_MAX		= 16;		//!< 静的サンプラー最大数


}
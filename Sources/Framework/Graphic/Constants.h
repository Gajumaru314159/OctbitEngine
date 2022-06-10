//***********************************************************
//! @file
//! @brief		グラフィック定数
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

	static const s32 BACKBUFFER_MAX			= 3;		//!< バックバッファの最大数
	static const s32 CBV_REGISTER_MAX		= 16;		//!< シェーダで利用できるコンスタント・バッファ・レジストリの最大数
	static const s32 SRV_REGISTER_MAX		= 128;		//!< シェーダで利用できるシェーダ・リソース・レジストリの最大数
	static const s32 UAV_REGISTER_MAX		= 16;		//!< シェーダで利用できるアンオーダー・アクセス・レジストリの最大数
	static const s32 SAMPLER_REGISTER_MAX	= 16;		//!< シェーダで利用できるサンプラー・ステート・レジストリの最大数

	static const s32 RENDER_TARstd::max		= 8;		//!< RenderTexture で使用できる描画先の最大数
	static const s32 VIEWPORT_MAX			= 16;		//!< ビューポート最大数
	static const s32 SCISSOR_RECT_MAX		= 4;		//!< シザー矩形最大数
	static const s32 MIPMAP__MAX			= 16;		//!< ミップマップ最大数

}// namespcae ob::graphic
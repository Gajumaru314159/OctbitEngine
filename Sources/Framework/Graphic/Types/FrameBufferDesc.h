//***********************************************************
//! @file
//! @brief		フレームバッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/RenderPass.h>
#include <Framework/Graphic/RenderTexture.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief		フレームバッファ定義
	//@―---------------------------------------------------------------------------
	struct FrameBufferDesc {
		RenderPass		renderPass;
		RenderTexture	renderTexture;		
		Size size;
		u32 layers;	//!< マルチビューを使用しない限り1
	};

}// namespcae ob::graphic
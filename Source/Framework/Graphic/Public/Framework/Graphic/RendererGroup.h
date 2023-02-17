﻿//***********************************************************
//! @file
//! @brief		メッシュバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/MeshData.h>

namespace ob::graphic {

	class IRenderer;

	//@―---------------------------------------------------------------------------
	//! @brief		RenderGroup
	//! @details
	//@―---------------------------------------------------------------------------
	class RenderGroup {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------

		RenderGroup& cull();
		RenderGroup& sort();

		void draw() {

		}

	private:
		IRenderer** m_renderers;
	};


}// namespcae ob
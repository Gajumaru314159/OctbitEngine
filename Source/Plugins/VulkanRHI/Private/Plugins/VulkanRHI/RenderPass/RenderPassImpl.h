﻿//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/IRenderPass.h>
#include <Framework/RHI/Types/RenderPassDesc.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::vulkan {

	class RenderPassImpl :public IRenderPass{
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		RenderPassImpl(VkDevice,const RenderPassDesc&);
		~RenderPassImpl();


		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		bool isValid()const override;

	private:

		RenderPassDesc m_desc;
		VkDevice		m_device = nullptr;
		VkRenderPass	m_renderPass = nullptr;

	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

}
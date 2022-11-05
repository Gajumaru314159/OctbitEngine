//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/IRenderPass.h>

namespace ob::rhi {

	OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(RenderPass);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のRenderPassへアクセス")

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderPass::RenderPass() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderPass::RenderPass(const RenderPassDesc& desc) {
		m_pImpl = Device::Get()->createRenderPass(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		if (!m_pImpl->isValid()) {
			LOG_FATAL_EX("Graphic", "RenderPassの生成に失敗");
			release();
		}
	}

}// namespace ob
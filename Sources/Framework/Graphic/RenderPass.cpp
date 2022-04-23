//***********************************************************
//! @file
//! @brief		レンダーパス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/RenderPass.h>
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Interface/IRenderPass.h>

namespace ob::graphic {

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
	RenderPass::RenderPass(const RenderPassDesc& desc,StringView name) {
		m_pImpl = Device::Get()->createRenderPass(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		m_pImpl->setName(name);
		if (!m_pImpl->isValid()) {
			LOG_ERROR_EX("Graphic", "RenderPassの生成に失敗");
			release();
		}
	}

}// namespace ob
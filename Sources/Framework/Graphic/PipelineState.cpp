//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/PipelineState.h>
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Interface/IPipelineState.h>

namespace ob::graphic {

	OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(PipelineState);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のPipelineStateへアクセス")

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineState::PipelineState() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineState::PipelineState(const PipelineStateDesc& desc) {
		m_pImpl = Device::Get()->createPipelineState(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		if (!m_pImpl->isValid()) {
			LOG_FATAL_EX("Graphic", "PipelineStateの生成に失敗");
			release();
		}
	}

}// namespace ob
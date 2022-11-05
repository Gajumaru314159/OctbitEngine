﻿//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/IFrameBuffer.h>

namespace ob::rhi {

	OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(FrameBuffer);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のPipelineStateへアクセス")

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	FrameBuffer::FrameBuffer() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	FrameBuffer::FrameBuffer(const FrameBufferDesc& desc) {
		m_pImpl = Device::Get()->createFrameBuffer(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		if (!m_pImpl->isValid()) {
			LOG_FATAL_EX("Graphic", "PipelineStateの生成に失敗");
			release();
		}
	}

}// namespace ob
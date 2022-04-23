//***********************************************************
//! @file
//! @brief		フレームバッファ
//! @author		Gajumaru
//***********************************************************
#include "FrameBuffer.h"
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Types/FrameBufferDesc.h>
#include <Framework/Graphic/Interface/IFrameBuffer.h>

namespace ob::graphic {

	OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(FrameBuffer);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のFrameBufferへアクセス")

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	FrameBuffer::FrameBuffer() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	FrameBuffer::FrameBuffer(const FrameBufferDesc& desc, StringView name) {
		m_pImpl = Device::Get()->createFrameBuffer(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		m_pImpl->setName(name);
		if (!m_pImpl->isValid()) {
			LOG_ERROR_EX("Graphic", "フレームバッファの生成に失敗");
			release();
		}
	}

}// namespace ob
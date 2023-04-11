//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferDesc& desc) {
		if (auto rhi= RHI::Get()) {
			return rhi->createFrameBuffer(desc);
		}
		return nullptr;
	}

}// namespace ob
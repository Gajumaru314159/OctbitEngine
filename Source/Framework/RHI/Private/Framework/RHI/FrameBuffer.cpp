//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferDesc& desc) {
		return Device::Get()->createFrameBuffer(desc);
	}

}// namespace ob
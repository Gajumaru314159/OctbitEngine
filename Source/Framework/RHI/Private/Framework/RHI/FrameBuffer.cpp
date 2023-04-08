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
		if (auto device = Device::Get()) {
			return device->createFrameBuffer(desc);
		}
		return nullptr;
	}

}// namespace ob
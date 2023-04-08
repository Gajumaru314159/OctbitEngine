//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<PipelineState> PipelineState::Create(const PipelineStateDesc& desc) {
		if (auto device = Device::Get()) {
			return device->createPipelineState(desc);
		}
		return nullptr;
	}

}// namespace ob
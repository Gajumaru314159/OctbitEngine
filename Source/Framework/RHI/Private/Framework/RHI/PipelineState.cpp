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
		return Device::Get()->createPipelineState(desc);
	}

}// namespace ob
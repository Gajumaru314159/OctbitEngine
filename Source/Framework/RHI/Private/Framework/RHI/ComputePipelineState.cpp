//***********************************************************
//! @file
//! @brief		コンピュートパイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/ComputePipelineState.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	//! @brief  生成
	Ref<ComputePipelineState> ComputePipelineState::Create(const ComputePipelineStateDesc& desc) {
		if (auto rhi = RHI::Get()) {
			return rhi->createComputePipelineState(desc);
		}
		return nullptr;
	}

}
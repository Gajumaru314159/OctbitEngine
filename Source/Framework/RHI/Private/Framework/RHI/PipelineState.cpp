//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<PipelineState> PipelineState::Create(const PipelineStateDesc& desc) {
		if (auto rhi= RHI::Get()) {
			return rhi->createPipelineState(desc);
		}
		return nullptr;
	}

}// namespace ob
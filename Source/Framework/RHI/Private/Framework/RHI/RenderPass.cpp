//***********************************************************
//! @file
//! @brief		レンダーパス
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<RenderPass> RenderPass::Create(const RenderPassDesc& desc) {
		if (auto rhi= RHI::Get()) {
			return rhi->createRenderPass(desc);
		}
		return nullptr;
	}

}// namespace ob
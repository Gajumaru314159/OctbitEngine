//***********************************************************
//! @file
//! @brief		レンダーパス
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<RenderPass> RenderPass::Create(const RenderPassDesc& desc) {
		return Device::Get()->createRenderPass(desc);
	}

}// namespace ob
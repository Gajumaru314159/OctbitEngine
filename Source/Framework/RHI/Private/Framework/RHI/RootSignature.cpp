//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<RootSignature> RootSignature::Create(const RootSignatureDesc& desc) {
		if (auto rhi= RHI::Get()) {
			return rhi->createRootSignature(desc);
		}
		return nullptr;
	}

}// namespace ob
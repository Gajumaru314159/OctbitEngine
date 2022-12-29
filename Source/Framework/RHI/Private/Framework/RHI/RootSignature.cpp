//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<RootSignature> RootSignature::Create(const RootSignatureDesc& desc) {
		return Device::Get()->createRootSignature(desc);
	}

}// namespace ob
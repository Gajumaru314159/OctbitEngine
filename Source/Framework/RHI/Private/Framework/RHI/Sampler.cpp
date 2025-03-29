//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    Ref<Sampler> Sampler::Create(const SamplerDesc& desc) {
		if (auto rhi = RHI::Get()) {
			return rhi->createSampler(desc);
		}
		return nullptr;
    }

}
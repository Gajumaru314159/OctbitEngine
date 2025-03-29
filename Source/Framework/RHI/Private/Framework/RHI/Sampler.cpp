//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/SystemResource.h>

namespace ob::rhi {

	static Ref<Sampler> GetPreset(PresetSampler type) {
		if (auto instance = SystemResource::Get()) {
			return instance->getPresetSampler(type);
		}
		return nullptr;
	}

	Ref<Sampler> Sampler::Default() {
		return GetPreset(PresetSampler::Default);
	}

    Ref<Sampler> Sampler::Create(const SamplerDesc& desc) {
		if (auto rhi = RHI::Get()) {
			return rhi->createSampler(desc);
		}
		return nullptr;
    }

}
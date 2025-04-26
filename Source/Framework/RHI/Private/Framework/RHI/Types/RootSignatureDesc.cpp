//***********************************************************
//! @file
//! @brief		ディスプレイ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/RootSignatureDesc.h>
#include <Framework/RHI/DescriptorLayout.h>

namespace ob::rhi {

	RootSignatureDesc::RootSignatureDesc(
		std::initializer_list<Ref<DescriptorLayout>> layouts,
		std::initializer_list<StaticSamplerDesc> samplers,
		const RootConstantsDesc& constants,
		RootSignatureFlags flags)
		: layouts(layouts)
		, samplers(samplers)
		, constants(constants)
		, flags(flags) {}

}
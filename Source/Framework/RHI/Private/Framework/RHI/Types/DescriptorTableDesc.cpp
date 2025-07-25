//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/DescriptorTableDesc.h>
#include <Framework/RHI/DescriptorLayout.h>

namespace ob::rhi {

	DescriptorTableDesc::DescriptorTableDesc(const Ref<DescriptorLayout>& layout) : layout(layout) {}

}
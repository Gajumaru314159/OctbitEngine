//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/DescriptorLayout.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    Ref<DescriptorLayout> DescriptorLayout::Create(const DescriptorLayoutDesc& desc) {
        if (auto rhi = Device::Get()) {
            return rhi->createDescriptorLayout(desc);
        }
        return nullptr;
    }

}
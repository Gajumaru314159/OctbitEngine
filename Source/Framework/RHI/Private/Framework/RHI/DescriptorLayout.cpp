//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/DescriptorLayout.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    Ref<DescriptorLayout> DescriptorLayout::Create(const DescriptorLayoutDesc& desc) {
        if (auto device = Device::Get()) {
            return device->createDescriptorLayout(desc);
        }
        return nullptr;
    }

}
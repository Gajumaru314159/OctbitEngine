//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    Ref<DescriptorTable> DescriptorTable::Create(const DescriptorTableDesc& desc) {
        if (auto device = Device::Get()) {
            return device->createDescriptorTable(desc);
        }
        return nullptr;
    }

}
//***********************************************************
//! @file
//! @brief		ディスプレイ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

    //! @brief      ディスプレイ定義
    struct DescriptorTableDesc {
        String name;
		Ref<DescriptorLayout> layout;

		DescriptorTableDesc() = default;
        DescriptorTableDesc(const Ref<DescriptorLayout>& layout);
    };

}
//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/DescriptorLayoutDesc.h>

namespace ob::rhi {

	struct BindlessHandle {
		BindingType type = BindingType::Unknown;
		u32			index = 0;
		u32			reserved0 = 0;
		u32			reserved1 = 0;


		bool operator==(const BindlessHandle& other) const {
			return memcmp(this, &other, sizeof(BindlessHandle)) == 0;
		}

		bool operator!=(const BindlessHandle& other) const {
			return !(*this == other);
		}
	};

}
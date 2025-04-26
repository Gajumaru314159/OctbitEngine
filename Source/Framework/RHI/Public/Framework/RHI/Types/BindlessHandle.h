//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/DescriptorLayoutDesc.h>

namespace ob::rhi {

	struct BindlessHandle {
		BindingType type;
		u32			index;
		u32			reserved0;
		u32			reserved1;


		bool operator==(const BindlessHandle& other) const {
			return memcmp(this, &other, sizeof(BindlessHandle)) == 0;
		}

		bool operator!=(const BindlessHandle& other) const {
			return !(*this == other);
		}
	};

}
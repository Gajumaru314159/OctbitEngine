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
	};

}
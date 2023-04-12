﻿//***********************************************************
//! @file
//! @brief		コマンドリスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Types/CommandListDesc.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<CommandList> CommandList::Create(const CommandListDesc& desc) {
		if (auto rhi= RHI::Get()) {
			return rhi->createCommandList(desc);
		}
		return nullptr;
	}

}
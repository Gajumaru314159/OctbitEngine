//***********************************************************
//! @file
//! @brief		コマンドリスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/Types/CommandListDesc.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  生成
	//@―---------------------------------------------------------------------------
	Ref<CommandList> CommandList::Create(const CommandListDesc& desc) {
		return Device::Get()->createCommandList(desc);;
	}

}// namespace ob
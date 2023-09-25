//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Command/OnetimeAllocator.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  メンバに持つ値はイミュータブルである必要がある。
	//@―---------------------------------------------------------------------------
	class ICommand {
	public:
		using CommandList = rhi::CommandList;
	public:
		virtual ~ICommand() = default;
		virtual void execute(CommandList&) = 0;
	protected:
		ICommand() = default;
	};

}
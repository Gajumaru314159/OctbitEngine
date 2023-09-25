//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Command/ICommand.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetViewportCommand : public ICommand{
	public:

		SetViewportCommand(OnetimeAllocator& allocator,Span<Viewport> viewports) {
			m_viewports = Span<Viewport>(allocator.allocate<Viewport>(viewports.size()),viewports.size());
			memcpy(m_viewports.data(), viewports.data(), viewports.size_bytes());
		}

		void execute(CommandList& commandList) override {
			commandList.setViewport(m_viewports.data(), m_viewports.size());
		}

	private:
		Span<Viewport> m_viewports;
	};

}
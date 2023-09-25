//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Viewport.h>
#include <Framework/Graphics/Command/OnetimeAllocator.h>
#include <Framework/Graphics/Command/CommandPriority.h>

namespace ob::graphics {


	class CommandAllocator {
	public:
		void* allocate(size_t size, size_t alugnment = 16);

	};

	//@―---------------------------------------------------------------------------
	//! @brief  コマンド記録
	//@―---------------------------------------------------------------------------
	class CommandRecorder {
	public:
		CommandRecorder();

		void setViewport(const Viewport* pViewport, s32 num) {
			addCommand<SetViewportCommand>(pViewports, num);
		}

	public:

		void* allocateBuffer(size_t size, size_t alignment = 16);

	private:
		template<class T,class... Args>
		T* addCommand(const CommandPriority& priority,Args&&... args) {
			auto buffer = m_allocator.allocate<T>();
			auto command = new(buffer)T(args...);
			m_commands.push_back(command);
			return command;
		}
	private:
		OnetimeAllocator m_allocator;
		Array<ICommand*> m_commands;
	};

}
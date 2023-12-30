//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Viewport.h>
#include <Framework/Graphics/Command/GraphicCommand.h>
#include <Framework/Graphics/Command/CommandPriority.h>

namespace ob::graphics {

	class ICommand;

	//@―---------------------------------------------------------------------------
	//! @brief		コマンドストレージ
	//@―---------------------------------------------------------------------------
	class CommandStorage {
	public:

		template<class T, class... Args>
		T* addCommand(const CommandPriority& priority, Args&&... args) {
			auto buffer = m_allocator.allocate<T>();
			auto command = new(buffer)T(args...);
			command->m_priority = m_priority;
			m_commands.push_back(command);
			return command;
		}

	private:
		Array<Blob>			m_blobs;
		Array<ICommand*>	m_commands;
	};

}
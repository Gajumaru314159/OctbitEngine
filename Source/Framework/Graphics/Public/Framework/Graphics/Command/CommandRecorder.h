//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Viewport.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Command/GraphicCommand.h>
#include <Framework/Graphics/Command/CommandPriority.h>

namespace ob::graphics {

	struct RenderArgs {
		RenderView& view;
	};

	class ICommand;

	//@―---------------------------------------------------------------------------
	//! @brief		コマンドストレージ
	//@―---------------------------------------------------------------------------
	class CommandStorage {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コマンドリストをマージする
		//@―---------------------------------------------------------------------------
		void merge(Array<ICommand*>& commands) {
			// TODO SwapのみにしてコピーはRenderThreadでするようにする
			m_commands.reserve(m_commands.size() + commands.size());
			std::move(commands.begin(), commands.end(), std::back_inserter(m_commands));
			commands.clear();
		}

		//@―---------------------------------------------------------------------------
		//! @brief		ネイティブコマンドを記録する
		//@―---------------------------------------------------------------------------
		void record(Ref<rhi::CommandList>& cmd) {
			if (!cmd)return;

			// TODO Thread分割
			std::stable_sort(m_commands.begin(), m_commands.end());

			for (auto command : m_commands) {
				command->execute(*cmd);
				command->~ICommand();
			}
			m_commands.clear();
		}

		void increment() {
			m_group++;
		}

	private:
		s32 m_group;
		Array<ICommand*> m_commands;
	};



	//@―---------------------------------------------------------------------------
	//! @brief		コマンド記録
	//! @details	CommandRecorderはスレッドごとに生成されます。
	//@―---------------------------------------------------------------------------
	class CommandRecorder {
	public:
		CommandRecorder(CommandStorage& storage);

		~CommandRecorder();

		void setPriority(u32 priority) {
			m_priority.offset = priority;
		}

		void beginRenderPass(const Ref<FrameBuffer>& frameBuffer) {
			// addCommand<BeginRenderPassCommand>(priority, frameBuffer);
		}
		void endRenderPass() {
			// addCommand<EndRenderPassCommand>(priority);
		}


		// void setViewport(const Viewport* pViewport, s32 num) {
		// 	addCommand<SetViewportCommand>(pViewport, num);
		// }

	public:

		void* allocateBuffer(size_t size, size_t alignment = 16);

	private:
		template<class T,class... Args>
		T* addCommand(const CommandPriority& priority,Args&&... args) {
			auto buffer = m_allocator.allocate<T>();
			auto command = new(buffer)T(args...);
			command->m_priority = m_priority;
			m_commands.push_back(command);
			return command;
		}
	private:
		CommandStorage& m_storage;
		CommandPriority m_priority;
		static thread_local Array<ICommand*> m_commands;
	};

}
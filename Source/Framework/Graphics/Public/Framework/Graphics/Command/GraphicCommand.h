//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/CommandList.h>
#include <Framework/Graphics/Command/CommandPriority.h>
#include <Framework/Graphics/Command/OnetimeAllocator.h>

namespace ob::graphics {

	using namespace ob::rhi;

	//@―---------------------------------------------------------------------------
	//! @brief  メンバに持つ値はイミュータブルである必要がある。
	//@―---------------------------------------------------------------------------
	class ICommand {
	public:
		virtual ~ICommand() = default;
		virtual void execute(CommandList&) = 0;


		bool operator<(const ICommand& rhs) const {
			return m_priority < rhs.m_priority;
		}
	protected:
		ICommand() = default;
	private:
		friend class CommandRecorder;
		CommandPriority m_priority;
	};




	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class BeginRenderPassCommand : public ICommand {
	public:

		BeginRenderPassCommand(OnetimeAllocator& allocator, const Ref<FrameBuffer>& frameBuffer) {
			m_frameBuffer = frameBuffer;
		}

		void execute(CommandList& commandList) override {
			commandList.beginRenderPass(m_frameBuffer);
		}

	private:
		Ref<FrameBuffer> m_frameBuffer;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class NextSubpassCommand : public ICommand {
	public:

		NextSubpassCommand(OnetimeAllocator& allocator) {
		}

		void execute(CommandList& commandList) override {
			commandList.nextSubpass();
		}

	};


	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class EndRenderPassCommand : public ICommand {
	public:

		EndRenderPassCommand(OnetimeAllocator& allocator) {
		}

		void execute(CommandList& commandList) override {
			commandList.endRenderPass();
		}

	};


	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetViewportCommand : public ICommand {
	public:

		SetViewportCommand(OnetimeAllocator& allocator, Span<Viewport> viewports) {
			m_viewports = Span<Viewport>(allocator.allocate<Viewport>(viewports.size()), viewports.size());
			memcpy(m_viewports.data(), viewports.data(), viewports.size_bytes());
		}

		void execute(CommandList& commandList) override {
			commandList.setViewport(m_viewports.data(), m_viewports.size());
		}

	private:
		Span<Viewport> m_viewports;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetScissorRectCommand : public ICommand {
	public:

		SetScissorRectCommand(OnetimeAllocator& allocator, Span<IntRect> scissors) {
			m_scissors = Span<IntRect>(allocator.allocate<IntRect>(scissors.size()), scissors.size());
			memcpy(m_scissors.data(), scissors.data(), scissors.size_bytes());
		}

		void execute(CommandList& commandList) override {
			commandList.setScissorRect(m_scissors.data(), m_scissors.size());
		}

	private:
		Span<IntRect> m_scissors;
	};

	// ClearColors
	// ClearDepthStencil

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetVertexBuffersCommand : public ICommand {
	public:

		SetVertexBuffersCommand(OnetimeAllocator& allocator, Span<Ref<Buffer>> buffers) {
			m_size = buffers.size();
			m_buffers = allocator.allocate<Ref<Buffer>>(buffers.size());
			for (s32 i = 0; i < m_size; ++i) {
				m_buffers[i] = buffers[i];
			}
		}

		~SetVertexBuffersCommand() {
			for (s32 i = 0; i < m_size; ++i) {
				m_buffers[i].reset();
			}
		}

		void execute(CommandList& commandList) override {
			commandList.setVertexBuffers(Span<Ref<Buffer>>(m_buffers, m_size));
		}

	private:
		size_t m_size;
		Ref<Buffer>* m_buffers;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetVertexBufferCommand : public ICommand {
	public:

		SetVertexBufferCommand(OnetimeAllocator& allocator, const Ref<Buffer> buffer) {
			m_buffer = buffer;
		}

		void execute(CommandList& commandList) override {
			commandList.setVertexBuffer(m_buffer);
		}

	private:
		Ref<Buffer> m_buffer;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetIndexBufferCommand : public ICommand {
	public:

		SetIndexBufferCommand(OnetimeAllocator& allocator, const Ref<Buffer> buffer) {
			m_buffer = buffer;
		}

		void execute(CommandList& commandList) override {
			commandList.setIndexBuffer(m_buffer);
		}

	private:
		Ref<Buffer> m_buffer;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetPipelineStateCommand : public ICommand {
	public:

		SetPipelineStateCommand(OnetimeAllocator& allocator, const Ref<PipelineState>& pipeline) {
			m_pipeline = pipeline;
		}

		void execute(CommandList& commandList) override {
			commandList.setPipelineState(m_pipeline);
		}

	private:
		Ref<PipelineState> m_pipeline;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class SetRootDesciptorTableCommand : public ICommand {
	public:

		SetRootDesciptorTableCommand(OnetimeAllocator& allocator, const SetDescriptorTableParam* params, s32 num) {
			if (params == nullptr)return;
			m_size = num;
			m_params = allocator.allocate<SetDescriptorTableParam>(num);
			for (s32 i = 0; i < m_size; ++i) {
				m_params[i] = params[i];
			}
		}

		~SetRootDesciptorTableCommand() {
			for (s32 i = 0; i < m_size; ++i) {
				m_params[i].table.reset();
			}
		}

		void execute(CommandList& commandList) override {
			commandList.setRootDesciptorTable(m_params, m_size);
		}

	private:
		s32 m_size;
		SetDescriptorTableParam* m_params;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class DrawCommand : public ICommand {
	public:

		DrawCommand(OnetimeAllocator& allocator, const DrawParam& param) {
			m_param = param;
		}

		void execute(CommandList& commandList) override {
			commandList.draw(m_param);
		}

	private:
		DrawParam m_param;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class DrawIndexedCommand : public ICommand {
	public:

		DrawIndexedCommand(OnetimeAllocator& allocator, const DrawIndexedParam& param) {
			m_param = param;
		}

		void execute(CommandList& commandList) override {
			commandList.drawIndexed(m_param);
		}

	private:
		DrawIndexedParam m_param;
	};





	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class PushMarkerCommand : public ICommand {
	public:

		PushMarkerCommand(OnetimeAllocator& allocator, StringView name) {
			m_name = { allocator.allocate<Char>(name.size()),name.size() };
		}

		void execute(CommandList& commandList) override {
			commandList.pushMarker(m_name);
		}

	private:
		StringView m_name;
	};
	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	class PopMarkerCommand : public ICommand {
	public:

		PopMarkerCommand(OnetimeAllocator& allocator) {
		}

		void execute(CommandList& commandList) override {
			commandList.popMarker();
		}

	};

}
//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/Graphics/FrameGraph/FGTexture.h>
#include <Framework/Graphics/FrameGraph/FGBuffer.h>
#include <Framework/Graphics/FrameGraph/FGFrameBuffer.h>
#include <fg/FrameGraph.hpp>
#include <fstream>

namespace ob::graphics {

	class FGResourcePool;

	//@―---------------------------------------------------------------------------
	//! @brief      FrameGraph中間テクスチャ
	//@―---------------------------------------------------------------------------
	class FG : Noncopyable, Nonmovable {
	public:
		
		using Builder = FrameGraph::Builder;
		struct NoData { };

	public:

		FG() = default;

		template <typename Data = NoData, typename Setup, typename Execute>
		const Data& addPass(StringView name, Setup&& setup, Execute&& execute) {

			U8String u8name;
			StringEncoder::Encode(name, u8name);

			return m_fg.addCallbackPass<Data>(
				u8name.str(),
				setup,
				[=](const Data& data, FrameGraphPassResources& resources, void* ctx) {
					auto& cmd = *static_cast<rhi::CommandList*>(ctx);
					execute(data, resources, cmd);
				}
			);
		}

		template <class T>
		const typename T::Desc& getDescriptor(FrameGraphResource id) const {
			return m_fg.getDescriptor(id);
		}


		FrameGraphResource import(const Ref<rhi::RenderTexture> & texture) {
			if (!texture)return {};
			return m_fg.import(texture->desc().name.str(), texture->descOfRenderTexture(), FGTexture{ texture });
		}

		FrameGraphResource import(const Ref<rhi::Buffer> & buffer) {
			if (!buffer)return {};
			return m_fg.import(buffer->getDesc().name.str(), buffer->getDesc(), FGBuffer{ buffer });
		}

		bool isValid(FrameGraphResource id) const {
			return m_fg.isValid(id);
		}

		void compile() {
			m_fg.compile();
		}

		void execute(rhi::CommandList& cmd, FGResourcePool& pool) {
			m_fg.execute(&cmd, &pool);
		}

		void debugOutput(StringView name) {
			std::ofstream f{name.data()};
			f << m_fg;
		}

	private:

		FrameGraph m_fg;

	};

	using FGBuilder = FrameGraph::Builder;
	using FGResources = FrameGraphPassResources;
	using FGResourceId = FrameGraphResource;

}
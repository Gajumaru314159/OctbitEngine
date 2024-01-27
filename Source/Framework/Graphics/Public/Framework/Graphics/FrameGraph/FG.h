//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/Graphics/FrameGraph/FGTexture.h>
#include <Framework/Graphics/FrameGraph/FGBuffer.h>
#include <Framework/Graphics/FrameGraph/FGFrameBuffer.h>
#include <fg/FrameGraph.hpp>

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
		const Data& addCallbackPass(StringView name, Setup&& setup, Execute&& execute) {

			U8String u8name;
			StringEncoder::Encode(name, u8name);

			return m_fg.addCallbackPass(
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

		template <class T>
		FrameGraphResource import(StringView name, const typename T::Desc& desc, T&& resource) {

			U8String u8name;
			StringEncoder::Encode(name, u8name);

			return m_fg.import(u8name.str(), desc, std::forward<T>(resource));
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

	private:

		FrameGraph m_fg;

	};

}
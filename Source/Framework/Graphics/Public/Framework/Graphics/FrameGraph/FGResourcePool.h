//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/FrameGraph/FGTexture.h>
#include <Framework/Graphics/FrameGraph/FGBuffer.h>

namespace ob::graphics {

	using namespace rhi;

	class FGResourcePool {
	public:

		FGResourcePool(::ob::rhi::RHI& rhi);

		void update();

		auto createTexture(const FGTexture::Desc& desc) -> Ref<RenderTexture>;
		auto createBuffer(const FGBuffer::Desc& desc) -> Ref<Buffer>;

		void destroyTexture(const FGTexture::Desc& desc, const Ref<rhi::RenderTexture>& texture);
		void destroyBuffer(const FGBuffer::Desc& desc, const Ref<rhi::Buffer>& buffer);

	private:

		template <typename T> struct ResourceEntry { Ref<T> resource; s32 life; };
		template <typename T> using ResourcePool = Array<ResourceEntry<T>>;

	private:
		rhi::RHI& m_rhi;

		HashMap<size_t, ResourcePool<RenderTexture>>	m_texturePools;
		HashMap<size_t, ResourcePool<Buffer>>			m_bufferPools;
	};

}
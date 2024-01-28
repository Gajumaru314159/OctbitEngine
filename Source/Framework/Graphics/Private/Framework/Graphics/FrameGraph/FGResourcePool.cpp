//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/Graphics/FrameGraph/FGTexture.h>
#include <Framework/Graphics/FrameGraph/FGBuffer.h>

namespace std {

	template <typename T, typename... Rest>
	static inline void hashCombine(std::size_t& seed, const T& v, const Rest &...rest) {
		// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
		seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		(hashCombine(seed, rest), ...);
	}

	template<>
	struct hash<::ob::rhi::RenderTextureDesc> {
		size_t operator()(const ::ob::rhi::RenderTextureDesc& desc) {
			size_t h = 0;
			//hashCombine(h, desc.format, desc.size.width, desc.size.height, desc.size.depth);
			return h;
		}
	};

	template<>
	struct hash<::ob::rhi::BufferDesc> {
		size_t operator()(const ::ob::rhi::BufferDesc& desc) {
			size_t h = 0;
			//hashCombine(h, desc.bufferType, desc.usage, desc.bufferSize, desc.bufferStride, desc.bufferFlags, desc.bindFlags);
			return h;
		}
	};

}


namespace ob::graphics {

	FGResourcePool::FGResourcePool(::ob::rhi::RHI& rhi)
		: m_rhi(rhi)
	{
	}

	void FGResourcePool::update() {

		static const s32 s_idleFrame = 10;

		for (auto& [hash, pool] : m_texturePools) {
			for (auto& [texture, life] : pool) {
				life++;
			}
			for (auto itr = pool.begin(); itr != pool.end();) {
				if (s_idleFrame < itr->life) {
					itr = pool.erase(itr);
				} else {
					itr++;
				}
			}
		}

		for (auto& [hash, pool] : m_bufferPools) {
			for (auto& [texture, life] : pool) {
				life++;
			}
			for (auto itr = pool.begin(); itr != pool.end();) {
				if (s_idleFrame < itr->life) {
					itr = pool.erase(itr);
				} else {
					itr++;
				}
			}
		}

	}

	auto FGResourcePool::createTexture(const FGTexture::Desc& desc) ->Ref<::ob::rhi::RenderTexture> {
		const auto hash = std::hash<FGTexture::Desc>{}(desc);
		auto& pool = m_texturePools[hash];
		if (pool.empty()) {
			return RenderTexture::Create(desc);
		} else {
			auto resource = pool.back().resource;
			pool.pop_back();
			return resource;
		}
	}
	auto FGResourcePool::createBuffer(const FGBuffer::Desc& desc) -> Ref<::ob::rhi::Buffer> {
		const auto hash = std::hash<FGBuffer::Desc>{}(desc);
		auto& pool = m_bufferPools[hash];
		if (pool.empty()) {
			return Buffer::Create(desc);
		} else {
			auto resource = pool.back().resource;
			pool.pop_back();
			return resource;
		}
	}
	
	void FGResourcePool::destroyTexture(const FGTexture::Desc& desc, const Ref<rhi::RenderTexture>& texture) {
		const auto h = std::hash<FGTexture::Desc>{}(desc);
		m_texturePools[h].push_back({ texture, 0 });
	}
	void FGResourcePool::destroyBuffer(const FGBuffer::Desc& desc, const Ref<rhi::Buffer>& buffer) {
		const auto h = std::hash<FGBuffer::Desc>{}(desc);
		m_bufferPools[h].push_back({ buffer, 0 });
	}

}
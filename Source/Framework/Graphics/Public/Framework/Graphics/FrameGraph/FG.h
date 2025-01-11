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
#include <fg/Blackboard.hpp>
#include <fstream>

namespace ob::graphics {

	class FGResourcePool;

	enum class FGTexture : s32 {};
	enum class FGBuffer : s32 {};

	using FGBlackboard = FrameGraphBlackboard;

	struct FGData {

		struct Pass {
			u32 id;
			String name;
			bool culled;
		};
		struct Resource {
			u32 id;
			String name;
			String desc;
			bool transient;
			u32 createdPassId;
			Vector<u32> readers;
			Vector<u32> writers;
		};

		Vector<Pass> passes;
		Vector<u32> reads;
		Vector<u32> writes;
		Map<u32, Resource> resources;
	};

	class FGDataWriter{
	public:
		FGDataWriter(FGData& data) 
			: m_data(data)
		{

		}

		void operator()(const PassNode& node,const std::vector<ResourceNode>& resourceNodes) {
			m_data.passes.push_back(
				FGData::Pass{
					node.getId(),
					node.getName().data(),
					!node.canExecute()
				}
			);
			const auto getResourceIds = [&](const auto type) {
				Vector<u32> ids;
				ids.reserve(resourceNodes.size());
				for (const auto [id, _] : node.each(type)) {
					ids.emplace_back(resourceNodes[id].getResourceId());
				}
				return ids;
			};
			m_data.reads = getResourceIds(PassNode::Read{});
			m_data.writes = getResourceIds(PassNode::Write{});
		}
		void operator()(const ResourceNode& node, const ResourceEntry& entry,const std::vector<PassNode>& passNodes) {
			
			auto [itr,added] = m_data.resources.try_emplace(entry.getId());
			auto& obj = itr->second;

			if (added) {
				obj = {
					entry.getId(),
					String(node.getName().data()),
					entry.toString(),
					entry.isTransient()
				};
			}
			for (const auto& p : passNodes) {
				if (p.creates(node.getId())) {
					obj.createdPassId = p.getId();
				}
				if (p.reads(node.getId())) {
					obj.readers.emplace_back(p.getId());
				}
				if (p.writes(node.getId())) {
					obj.writers.emplace_back(p.getId());
				}
			}
		}
		void flush(std::ostream& os) const { }
	private:
		FGData& m_data;
	};


	//! @brief      FrameGraph
	class FG : Noncopyable, Nonmovable {
	public:

		FG() = default;

		//! @brief      パスを追加
		template <typename Data, typename Setup, typename Execute>
		const Data& addPass(StringView name, Setup&& setup, Execute&& execute);

		//! @brief      FGTextureのRenderTextureDescを取得する
		const rhi::RenderTextureDesc& getDesc(FGTexture texture) {
			return m_fg.getDescriptor<FGTextureInstance>(static_cast<FrameGraphResource>(texture));
		}

		//! @brief      FGBufferのBufferDescを取得する
		const rhi::BufferDesc& getDesc(FGBuffer buffer) {
			return m_fg.getDescriptor<FGBufferInstance>(static_cast<FrameGraphResource>(buffer));
		}

		//! @brief      RenderTextureをインポートする
		FGTexture import(const Ref<rhi::RenderTexture> & texture) {
			if (!texture)return FGTexture{-1};
			return static_cast<FGTexture>(m_fg.import(texture->desc().name.str(), texture->descOfRenderTexture(), FGTextureInstance{ texture }));
		}

		//! @brief      Bufferをインポートする
		FGBuffer import(const Ref<rhi::Buffer> & buffer) {
			if (!buffer)return FGBuffer{-1};
			return static_cast<FGBuffer>(m_fg.import(buffer->getDesc().name.str(), buffer->getDesc(), FGBufferInstance{ buffer }));
		}

		//! @brief      FGTextureが有効な値か
		bool isValid(FGTexture id) const {
			return m_fg.isValid(static_cast<FrameGraphResource>(id));
		}

		//! @brief      FGBufferが有効な値か
		bool isValid(FGBuffer id) const {
			return m_fg.isValid(static_cast<FrameGraphResource>(id));
		}

		//! @brief      FrameGraphをコンパイルして実行可能な状態にする
		//! @details	追加されたパスの依存関係を考慮してソートし、必要なパスのみを実行します
		void compile() {
			m_fg.compile();
		}

		//! @brief     コンパイルされたパスを実行する
		void execute(rhi::CommandList& cmd, FGResourcePool& pool) {
			m_fg.execute(&cmd, &pool);
		}

		//! @brief      dot形式でFrameGraphの依存関係を出力する
		void save(StringView name) {
			std::ofstream f{name.data()};
			f << m_fg;
		}

		template<class Writer>
		void debugOutput(Writer&& writer) {
			std::stringstream s;
			m_fg.debugOutput<Writer>(s,writer);
		}

	private:
		FrameGraph m_fg;
	};

	//! @brief		FrameGraphのリソース管理クラス
	//! @details	FGTextureやFGBufferからRenderTextureやBufferへのアクセスする手段を
	//!				提供します。
	class FGResources {
	public:

		//! @brief      コンストラクタ
		FGResources(FrameGraphPassResources& resources)
			: m_resources(resources)
		{
		}

		//! @brief      FGTextureからRenderTextureのインスタンスを取得する
		Ref<rhi::RenderTexture> get(FGTexture texture) {
			return m_resources.get<FGTextureInstance>(static_cast<FrameGraphResource>(texture)).instance;
		}

		//! @brief      FGBufferからBufferのインスタンスを取得する
		Ref<rhi::Buffer> get(FGBuffer buffer) {
			return m_resources.get<FGBufferInstance>(static_cast<FrameGraphResource>(buffer)).instance;
		}

		//! @brief      FGTextureのRenderTextureDescを取得する
		const rhi::RenderTextureDesc& getDesc(FGTexture texture) {
			return m_resources.getDescriptor<FGTextureInstance>(static_cast<FrameGraphResource>(texture));
		}

		//! @brief      FGBufferのBufferDescを取得する
		const rhi::BufferDesc& getDesc(FGBuffer buffer) {
			return m_resources.getDescriptor<FGBufferInstance>(static_cast<FrameGraphResource>(buffer));
		}

	private:
		FrameGraphPassResources& m_resources;
	};

	//! @brief      FrameGraphで使用するリソースを生成するクラス
	class FGBuilder {
	public:
		FGBuilder(FrameGraph::Builder& builder) 
			: m_builder(builder)
		{
		}

		FGTexture create(const rhi::RenderTextureDesc& desc) {
			return static_cast<FGTexture>(m_builder.create<FGTextureInstance>(std::string_view(desc.name.data(), desc.name.size()), desc));
		}

		FGBuffer create(const rhi::BufferDesc& desc) {
			return static_cast<FGBuffer>(m_builder.create<FGBufferInstance>(std::string_view(desc.name.data(), desc.name.size()), desc));
		}

		FGTexture read(FGTexture id, u32 flags = 0) {
			return static_cast<FGTexture>(m_builder.read(static_cast<FrameGraphResource>(id), flags));
		}

		FGBuffer read(FGBuffer id, u32 flags = 0) {
			return static_cast<FGBuffer>(m_builder.read(static_cast<FrameGraphResource>(id), flags));
		}

		FGTexture write(FGTexture id, u32 flags = 0) {
			return static_cast<FGTexture>(m_builder.write(static_cast<FrameGraphResource>(id), flags));
		}

		FGBuffer write(FGBuffer id, u32 flags = 0) {
			return static_cast<FGBuffer>(m_builder.write(static_cast<FrameGraphResource>(id), flags));
		}

		FGBuilder& setSideEffect() {
			m_builder.setSideEffect();
			return *this;
		}

	private:
		FrameGraph::Builder& m_builder;
	};


	//! @brief      パスを追加
	template <typename Data, typename Setup, typename Execute>
	const Data& FG::addPass(StringView name, Setup&& setup, Execute&& execute) {
		return m_fg.addCallbackPass<Data>(
			name,
			[&](FrameGraph::Builder& nativeBuilder, Data& data) {
				FGBuilder builder(nativeBuilder);
				setup(builder, data);
			},
			[=](const Data& data, FrameGraphPassResources& nativeResources, void* ctx) {
				auto& cmd = *static_cast<rhi::CommandList*>(ctx);
				FGResources resources(nativeResources);
				execute(data, resources, cmd);
			}
		);
	}

}
//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <fg/FrameGraph.hpp>

namespace ob::graphics {

	class FGResourcePool;

	//! @brief FrameGraphで使用されるリソースの種類
	enum class FGResourceType : s32 {
		Invalid,
		Texture,
		Buffer,
		Dummy,
	};

	//! @brief フレームグラフリソースを表す構造体
	struct FGResource {
		FGResourceType type = FGResourceType::Invalid;
		FrameGraphResource value = -1;

		operator bool()const {
			return type != FGResourceType::Invalid;
		}
	};



	//! @brief		FrameGraphのリソース管理クラス
	//! @details	FGResourceやFGResourceからRenderTextureやBufferへのアクセスする手段を提供します。 
	//!				内部的にはリソースプール(FGResourcePool)が管理しているリソースを参照します。
	class FGResources {
	public:

		//! @brief      コンストラクタ
		FGResources(FrameGraphPassResources& resources);

		//! @brief      FGResourceからRenderTextureのインスタンスを取得する
		//! @details	texture.type がFGResourceType::Textureでない場合はアサートで停止します。
		Ref<rhi::RenderTexture> getTexture(FGResource texture);

		//! @brief      FGResourceからBufferのインスタンスを取得する
		//! @details    texture.type がFGResourceType::Bufferでない場合はアサートで停止します。
		Ref<rhi::Buffer> getBuffer(FGResource buffer);

		//! @brief      FGResourceのRenderTextureDescを取得する
		//! @details    texture.type がFGResourceType::Textureでない場合はアサートで停止します。
		const rhi::RenderTextureDesc& getTextureDesc(FGResource texture);

		//! @brief      FGResourceのBufferDescを取得する
		//! @details    buffer.type がFGResourceType::Bufferでない場合はアサートで停止します。
		const rhi::BufferDesc& getBufferDesc(FGResource buffer);

	private:
		FrameGraphPassResources& m_resources;
	};

	//! @brief      FrameGraphで使用するリソースを生成するクラス
	//! @details	フレーム内でのみ使用するリソースを生成するためのクラスです。  
	//!				create() で生成したリソースは　FrameGraph::execute()を呼び出すまでTextureやBufferといったリソースの実体を持ちません。  
	//!				実体の取得は FGResources を通して遅延実行されます。
	class FGBuilder {
	public:
		FGBuilder(FrameGraph::Builder& builder);

		FGResource create(const rhi::RenderTextureDesc& desc);

		FGResource create(const rhi::BufferDesc& desc);

		FGResource createDummy();

		FGResource read(FGResource resource, u32 flags = 0);

		FGResource write(FGResource resource, u32 flags = 0);

		FGBuilder& setSideEffect();

	private:
		FrameGraph::Builder& m_builder;
	};

	class FGConnections {
	public:
		void connect(StringView from, StringView to) {
			m_connections.emplace(to, from);
		}
		FGResource get(StringView name) {
			return m_outputs[m_connections[name.data()]];
		}
		void set(StringView name,FGResource resource) {
			m_outputs.emplace(name, resource);
		}
	private:
		Map<String, String,std::less<>> m_connections;
		Map<String, FGResource, std::less<>> m_outputs;
	};


	//! @brief      FrameGraph
	class FG : Noncopyable, Nonmovable {
	private:
		template<typename TSetup, typename TData>
		using Setupable = std::is_invocable<TSetup, FGBuilder&, TData&>;
		template<typename TExecute, typename TData>
		using Executable = std::is_invocable<TExecute, const TData&, FGResources&, Ref<rhi::CommandList>&>;
		template<typename TData, typename TSetup>
		using IsValid1 = std::enable_if_t<Setupable<TSetup, TData>::value, const TData&>;
		template<typename TData, typename TSetup, typename TExecute>
		using IsValid2 = std::enable_if_t<Setupable<TSetup, TData>::value && Executable<TExecute, TData>::value, const TData&>;
	public:
		struct NoData {};
	public:

		FG() = default;

		//! @brief      パスを追加
		template <typename Data, typename Setup>
		auto addPass(StringView name, Setup&& setup) -> IsValid1<Data, Setup>;

		//! @brief      パスを追加
		template <typename Data, typename Setup, typename Execute>
		auto addPass(StringView name, Setup&& setup, Execute&& execute) -> IsValid2<Data, Setup, Execute>;

		//! @brief      FGResourceのRenderTextureDescを取得する
		const rhi::RenderTextureDesc& getTextureDesc(FGResource texture);

		//! @brief      FGResourceのBufferDescを取得する
		const rhi::BufferDesc& getBufferDesc(FGResource buffer);

		//! @brief      RenderTextureをインポートする
		FGResource import(const Ref<rhi::RenderTexture>& texture);

		//! @brief      Bufferをインポートする
		FGResource import(const Ref<rhi::Buffer>& buffer);

		//! @brief      FGResourceが有効な値か
		bool isValid(FGResource resource) const;

		//! @brief      FrameGraphをコンパイルして実行可能な状態にする
		//! @details	追加されたパスの依存関係を考慮してソートし、必要なパスのみを実行します
		void compile();

		//! @brief     コンパイルされたパスを実行する
		void execute(Ref<rhi::CommandList>& cmd, FGResourcePool& pool);

		//! @brief      dot形式でFrameGraphの依存関係を出力する
		void save(StringView name);

		template<class Writer>
		void debugOutput(Writer&& writer) {
			std::stringstream s;
			m_fg.debugOutput<Writer>(s,writer);
		}

	private:
		FrameGraph m_fg;
	};


	//! @brief      パスを追加
	template <typename Data, typename Setup>
	auto FG::addPass(StringView name, Setup&& setup) -> IsValid1<Data, Setup> {
		return m_fg.addCallbackPass<Data>(
			name,
			[&](FrameGraph::Builder& nativeBuilder, Data& data) {
				FGBuilder builder(nativeBuilder);
				setup(builder, data);
			},
			[=](const Data& data, FrameGraphPassResources& nativeResources, void* ctx) {
			}
		);
	}


	//! @brief      パスを追加
	template <typename Data, typename Setup, typename Execute>
	auto FG::addPass(StringView name, Setup&& setup, Execute&& execute) -> IsValid2<Data, Setup, Execute> {
		return m_fg.addCallbackPass<Data>(
			name,
			[&](FrameGraph::Builder& nativeBuilder, Data& data) {
				FGBuilder builder(nativeBuilder);
				setup(builder, data);
			},
			[=](const Data& data, FrameGraphPassResources& nativeResources, void* ctx) {
				auto& cmd = *static_cast<Ref<rhi::CommandList>*>(ctx);
				FGResources resources(nativeResources);
				execute(data, resources, cmd);
			}
		);
	}

}
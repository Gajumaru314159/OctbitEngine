//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Material/MaterialDesc.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::graphics {

	struct MaterialSystemDesc {
		MaterialPropertiesSetDesc properties;
	};

	//! @brief マテリアルシステム
	//! 
	//! マテリアルを使用するために必要な管理機構を提供します。
	//! 事前に各RenderFeatureから必要なマテリアルのプロパティを収集して初期化することで、RenderSceneやRenderViewの生成時に
	//! 自動的に MaterialBlock を生成します。
	class MaterialSystem : public Singleton<MaterialSystem> {
	public:
		struct MaterialLayoutSet {
			Ref<rhi::DescriptorLayout> global;
			Ref<rhi::DescriptorLayout> scene;
			Ref<rhi::DescriptorLayout> view;
		};
		struct MateriaBlockDescSet {
			MaterialBlockDesc global;
			MaterialBlockDesc scene;
			MaterialBlockDesc view;
		};
		using VertexLayout = ob::rhi::VertexLayout;
	public:

		MaterialSystem(const MaterialSystemDesc& desc);

		const MaterialLayoutSet& getLayouts() const { return m_layouts; }

		MaterialBlock createSceneBlock(StringView name) const;

		MaterialBlock createViewBlock(StringView name) const;

		MaterialBlock& getGlobalBlock() { return m_globalBlock; }
	private:

		MaterialLayoutSet		m_layouts;
		MateriaBlockDescSet		m_descs;
		MaterialBlock			m_globalBlock;	

		SpinLock				m_vertexLayoutsLock;
		Vector<VertexLayout>	m_vertexLayouts;
	};


	struct MaterialBlockSet {
		MaterialBlock* global = nullptr;
		MaterialBlock* scene = nullptr;
		MaterialBlock* view = nullptr;
	};








	class Material2 : public RefObject {
	public:
		using PipelineState = ob::rhi::PipelineState;
		using RootSignature = ob::rhi::RootSignature;
	public:
		static Ref<Material2> Create(const MaterialDesc& desc);
	public:

		//! @brief Meshの描画に必要なリソースを事前確保する
		bool prepare(const Ref<Mesh>& mesh);

		void record(Ref<rhi::CommandList>& commandList, MaterialBlockSet& blocks, const Ref<Mesh>& mesh, s32 submesh, StringView pass,s32 quality);

	private:
		
		Material2(const MaterialDesc& desc);

		Ref<PipelineState> createPipeline(StringView pass,s32 quality, const rhi::VertexLayout& vertexLayout, VertexLayoutId id);

	private:
		struct PipelineKey {
			String pass;
			s32 qualityIndex;
			VertexLayoutId layout;
			bool operator==(const PipelineKey& rhs)const { return pass == rhs.pass && qualityIndex == rhs.qualityIndex && layout == rhs.layout; }
			bool operator!=(const PipelineKey& rhs)const { return !(*this == rhs); }
		};
		struct PipelineKeyHasher {
			size_t operator()(const PipelineKey& v)const { return std::hash<decltype(v.pass)>{}(v.pass) ^ std::hash<decltype(v.qualityIndex)>{}(v.qualityIndex) ^ std::hash<decltype(v.layout)>{}(v.layout); }
		};
		using PipelineMap = HashMap<PipelineKey, Ref<rhi::PipelineState>, PipelineKeyHasher>;
	private:
		MaterialDesc		m_desc;			//!< 描画パスを含むマテリアルの説明
		Ref<RootSignature>  m_signature;	//!< MaterialDesc に対応する RootSignature
		MaterialBlock       m_block;		//!< Material 毎のプロパティブロック

		SpinLock		    m_pipelinesLock;
		PipelineMap	        m_pipelines;

	};

}
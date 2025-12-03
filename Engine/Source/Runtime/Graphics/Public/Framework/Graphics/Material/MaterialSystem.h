//***********************************************************
//! @file
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
		
		MaterialBlockSet();
		MaterialBlockSet(RenderView& scene);
		MaterialBlockSet(RenderScene& scene);
	};

}
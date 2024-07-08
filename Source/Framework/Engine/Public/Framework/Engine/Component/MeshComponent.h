//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Graphics/Forward.h>
// #include <Framework/Graphics/Model/ModelRenderHandle.h>

namespace ob::engine {

	class RenderLayer {
	public:
		void set(s32 index, bool flag);
		bool get(s32 index, bool fallback);
		void set(StringView name, bool flag);
		bool get(StringView index, bool fallback);
	};

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class MeshComponent : public engine::Component {
	public:

		MeshComponent();

		void setModel(StringView path);

		void activate() override;
		void deactivate() override;
	private:
		void updateModel();
	private:
		//graphics::ModelRenderHandle m_handle;
		Ref<graphics::Material> m_material;
		Ref<rhi::Texture> m_mainTex;
		Ref<graphics::Mesh> m_mesh;
		String m_path;
	};

}
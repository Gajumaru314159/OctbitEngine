//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Graphics/Forward.h>
// #include <Framework/Graphics/Model/ModelRenderHandle.h>

#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Engine/Component/TransformComponent.h>

#include <Framework/Model/Model.h>


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
		OB_RTTI();

		MeshComponent(Entity&);

		void setModel(StringView path);
		auto getModel()const -> const String&;

	private:
		void initialize() override;
		void activate() override;
		void deactivate() override;
	private:
		void updateModel();
		void onTransformChanged(TransformComponent&);
	private:
		Ref<model::Model> m_model;
		Vector<graphics::MaterialId> m_materialIds;

		String m_path;
		TransformChangedHandle m_hTransformChanged;
	};

}
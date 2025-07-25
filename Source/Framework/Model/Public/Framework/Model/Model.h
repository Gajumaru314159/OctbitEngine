//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Render/RenderScene.h>

#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>
#include <Framework/Model/ModelData.h>

namespace ob::model {

	struct ModelData;

	//! @brief  説明
	class Model : public RefObject {
	public:
		using Material = graphics::Material;
		using RenderScene = graphics::RenderScene;
	public:

		static Ref<Model> Create(const ModelData&);
		static Ref<Model> Create(StringView name);
		static Ref<Model> Load(StringView path);

	public:

		void join(RenderScene&);
		void leaveScene();

		void setTransform(Transform transform, StringView bone);

		auto getBounds()const->Box;
		void setBounds(Box box);

		bool getActive()const;
		void setActive(bool);

		auto findMaterial(StringView name)const->Ref<Material>;
		void setMaterial(StringView name,const Ref<Material>&);

		void visitParts(Func<void(ModelParts&)>);

	private:

		Model();
		void finalize()override;

		void createRenderUnit();

	private:
		
		graphics::RenderScene*			m_scene = nullptr;
		ModelData						m_data;


		Vector<graphics::MaterialId>	m_materialIds;
	};




}
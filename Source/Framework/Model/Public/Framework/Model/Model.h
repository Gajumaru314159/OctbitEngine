//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Render/RenderScene.h>

#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::model {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Model : public RefObject {
	public:
		using Material = graphics::Material;
		using RenderScene = graphics::RenderScene;
	public:

		static Ref<Model> Create(StringView name);
		static Ref<Model> Load(StringView path);

	public:

		void join(RenderScene&);
		void leaveScene();

		auto getMesh()const->Ref<graphics::Mesh>;
		void setMesh(const Ref<graphics::Mesh> mesh);

		void setTransform(Transform transform, StringView bone);

		auto getBounds()const->Box;
		void setBounds(Box box);

		bool getActive()const;
		void setActive(bool);

		Array<Ref<Material>>& getMaterials();

		auto findMaterial(StringView name)const->Ref<Material>;
		void setMaterial(StringView name,const Ref<Material>&);

	private:

		Model();
		void finalize()override;

		void createRenderUnit();

	private:
		
		graphics::RenderScene*			m_scene = nullptr;
		Ref<graphics::Mesh>				m_mesh;
		Array<Ref<graphics::Material>>	m_materials;
		Map<String, s32,std::less<>>	m_materialMap;

		u32 m_modelId;

	};




}
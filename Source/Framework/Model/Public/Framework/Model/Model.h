//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Graphics/Mesh.h>
#include <Framework/Graphics/Render/RenderScene.h>

#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::model {

	class ModelRenderFeature : public graphics::RenderFeature{
	public:
		void addModel(Model* model);
		void removeModel(Model* model);
	};

	struct ModelData;

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class ModelPart {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		ModelPart();

	private:



	};

	class MeshObject {
	public:

	private:
		Ref<graphics::Mesh> m_mesh;

	};

	enum class ShadowCastingMode {
		Off,
		On,
		TwoSided,
		ShadowOnly,
	};



	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Model : public RefObject {
	public:
		using Material = graphics::Material;
		using RenderScene = graphics::RenderScene;
	public:

		static Ref<Model> Create(StringView name);
		static Ref<Model> Load(Path path);

	public:

		void join(const Ref<RenderScene>&);
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

		bool getReceiveShadow()const;
		void setReceiveShadow(bool);

		void setScene(const Ref<RenderScene>& scene) {
			if (m_scene == scene)return;

			if (m_scene) {
				if (auto feature = m_scene->findFeature<ModelRenderFeature>()) {
					feature->removeModel(this);
				}
			}

			m_scene = scene;

			if (m_scene) {
				if (auto feature = m_scene->findFeature<ModelRenderFeature>()) {
					feature->addModel(this);
				}
			}
		}

		// RenderingLayerMask
	private:

		Model();
		void finalize()override;

	private:
		
		Ref<graphics::RenderScene>		m_scene;
		Ref<graphics::Mesh>				m_mesh;
		Array<Ref<graphics::Material>>	m_materials;
		Map<String, s32,std::less<>>	m_materialMap;

		u32 m_modelId;

	};

}
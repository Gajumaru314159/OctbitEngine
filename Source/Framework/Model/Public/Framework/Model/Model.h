//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Mesh.h>

namespace ob::model {

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
		rendering::Mesh m_mesh;

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
	class Model {
	public:
		using Material = graphics::Material;
		using RenderScene = graphics::RenderScene;
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		Model(RenderScene&);

		auto getMesh()const->Ref<graphics:Mesh>;
		void setMesh(Ref<graphics::Mesh> mesh);

		void setTransform(Transform transform, StringView bone);

		auto getBounds()const->Box;
		void setBounds(Box box);

		bool getActive()const;
		void setActive(bool);

		Array<Ref<Material>> getMaterials();

		auto findMaterial(StringView name)const->Ref<Material>;
		void setMaterial(StringView name,Ref<Material>);

		bool getReceiveShadow()const;
		void setReceiveShadow(bool);

		// RenderingLayerMask


	private:

		RenderScene& m_scene;
		u32 m_modelId;

	};

}
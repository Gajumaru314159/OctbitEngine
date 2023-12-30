//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Model/Model.h>

namespace ob::model {

	Model::Model(RenderScene& scene)
		: m_scene(scene)
	{

		if (auto feature = m_scene.findFeature<ModelRenderFeature>()) {
			m_modelId = feature->createModel();
		}

	}

	auto Model::getMesh()const->Ref<graphics:Mesh> {
		return nullptr;
	}
	void Model::setMesh(Ref<graphics::Mesh> mesh) {

	}

	void Model::setTransform(Transform transform, StringView bone) {

	}

	auto Model::getBounds()const->Box {
		return {};
	}
	void Model::setBounds(Box box) {
	}

	bool Model::getActive()const {
		return false;
	}
	void Model::setActive(bool) {

	}

	Array<Ref<Material>> Model::getMaterials() {
		return { };
	}

	auto Model::findMaterial(StringView name)const->Ref<Material> {
		return nullptr;
	}
	void Model::setMaterial(StringView name, Ref<Material>) {
	}

	bool Model::getReceiveShadow()const {
		return false;
	}
	void Model::setReceiveShadow(bool) {

	}

}
//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Model/Model.h>

namespace ob::model {

	Ref<Model> Model::Create(StringView name) {
		return nullptr;
	}
	Ref<Model> Model::Load(Path path) {
		return nullptr;
	}

	Model::Model()
	{

	}

	void Model::finalize(){
		leaveScene();
		
		// TODO 即時解放に問題がある場合はRenderFeatureに削除登録
		delete this;
	}

	void Model::join(const Ref<RenderScene>& scene) {

		leaveScene();

		m_scene = scene;

		if (m_scene) {
			if (auto feature = m_scene->findFeature<ModelRenderFeature>()) {
				m_modelId = feature->addModel(this);
			}
		}

	}
	void Model::leaveScene() {
		if (m_scene) {
			if (auto feature = m_scene->findFeature<ModelRenderFeature>()) {
				feature->removeModel(this);
			}
		}
		m_modelId = 0;
	}

	auto Model::getMesh()const->Ref<graphics::Mesh> {
		return m_mesh;
	}
	void Model::setMesh(const Ref<graphics::Mesh> mesh) {
		m_mesh = mesh;
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

	Array<Ref<graphics::Material>>& Model::getMaterials() {
		return m_materials;
	}

	auto Model::findMaterial(StringView name)const->Ref<Material> {
		auto found = m_materialMap.find(name);
		if (found == m_materialMap.end())return {};
		return m_materials.at(found->second);
	}
	void Model::setMaterial(StringView name, const Ref<Material>& material) {
		auto found = m_materialMap.find(name);
		if (found == m_materialMap.end())return;
		m_materials.at(found->second) = material;
	}

	bool Model::getReceiveShadow()const {
		return false;
	}
	void Model::setReceiveShadow(bool) {

	}

}
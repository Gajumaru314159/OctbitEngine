//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Model/Model.h>
#include <Framework/Core/String/StringEncoder.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <Framework/Graphics/Mesh/MeshData.h>
#include <Framework/Graphics/Builtin/RenderFeature/ModelRenderFeature.h>
#include <Framework/Graphics/Material/Material.h>

namespace ob::model {

	Ref<Model> Model::Create(StringView name) {
		return nullptr;
	}
	Ref<Model> Model::Load(StringView path) {

		Ref<Model> model = new Model();

		std::string pathStr(path.data());

		Assimp::Importer importer;
		auto scene = importer.ReadFile(pathStr,0);

		if (scene == nullptr) {
			LOG_ERROR("Failed to load {}",path);
			return nullptr;
		}

		for (auto mesh : Span<aiMesh*>(scene->mMeshes, scene->mNumMeshes)) {

			if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
				LOG_ERROR("非対応のプリミティブライプです");
				continue;
			}


			graphics::MeshData meshData;
			meshData.name = mesh->mName.C_Str();

			// 頂点
			if (mesh->mVertices) {
				for (auto v : Span<aiVector3D>(mesh->mVertices, mesh->mNumVertices)) {
					meshData.positions.emplace_back(v.x, v.y, v.z);
				}
			}
			if (mesh->mNormals) {
				for (auto v : Span<aiVector3D>(mesh->mNormals, mesh->mNumVertices)) {
					meshData.normals.emplace_back(v.x, v.y, v.z);
				}
			}
			if (mesh->mTangents) {
				for (auto v : Span<aiVector3D>(mesh->mTangents, mesh->mNumVertices)) {
					meshData.tangents.emplace_back(v.x, v.y, v.z,1.0f);
				}
			}
			if (mesh->mColors[0]) {
				for (auto c : Span<aiColor4D>(mesh->mColors[0], mesh->mNumVertices)) {
					meshData.colors.emplace_back(c.r, c.g, c.b, c.a);
				}
			}
			if (mesh->mTextureCoords[0]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[0], mesh->mNumVertices)) {
					meshData.uvs.emplace_back(v.x, v.y);
				}
			}
			if (mesh->mTextureCoords[1]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[1], mesh->mNumVertices)) {
					meshData.uvs1.emplace_back(v.x, v.y);
				}
			}
			if (mesh->mTextureCoords[2]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[2], mesh->mNumVertices)) {
					meshData.uvs2.emplace_back(v.x, v.y);
				}
			}
			if (mesh->mTextureCoords[3]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[3], mesh->mNumVertices)) {
					meshData.uvs3.emplace_back(v.x, v.y);
				}
			}

			// インデックス
			for (auto face : Span<aiFace>(mesh->mFaces, mesh->mNumFaces)) {
				if (face.mNumIndices == 3) {
					meshData.indices.emplace_back(face.mIndices[0]);
					meshData.indices.emplace_back(face.mIndices[1]);
					meshData.indices.emplace_back(face.mIndices[2]);
				}
			}

			model->m_mesh = graphics::Mesh::Create(std::move(meshData));

			break;
		}


		// マテリアル
		for (auto m : Span<aiMaterial*>(scene->mMaterials, scene->mNumMaterials)) {
			model->m_materialMap[m->GetName().C_Str()] = 0;
		}
		model->m_materials.resize(model->m_materialMap.size());

		// アニメーション

		// テクスチャ

		// AABB

		return model;
	}

	Model::Model()
	{

	}

	void Model::finalize(){
		leaveScene();
		
		// TODO 即時解放に問題がある場合はRenderFeatureに削除登録
		delete this;
	}

	void Model::join(RenderScene& scene) {

		leaveScene();

		m_scene = &scene;

		if (m_scene) {
			if (auto feature = m_scene->findFeature<graphics::ModelRenderFeature>()) {
				//m_modelId = feature->addModel(this);
			}
		}

	}
	void Model::leaveScene() {
		if (m_scene) {
			if (auto feature = m_scene->findFeature<graphics::ModelRenderFeature>()) {
			//	feature->removeModel(this);
			}
		}
		m_scene = nullptr;
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

	Vector<Ref<graphics::Material>>& Model::getMaterials() {
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

	void Model::createRenderUnit() {

	}

}
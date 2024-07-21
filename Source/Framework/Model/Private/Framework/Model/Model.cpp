//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/Bitmap.h>
#include <Framework/Model/Model.h>
#include <Framework/Core/String/StringEncoder.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Framework/Graphics/Mesh/MeshData.h>
#include <Framework/Graphics/Builtin/RenderFeature/ModelRenderFeature.h>
#include <Framework/Graphics/Material/Material.h>

namespace ob::model {

	Ref<Model> Model::Create(StringView name) {
		return nullptr;
	}
	Ref<Model> Model::Load(StringView path) {

		String directory = Path::Parent(path);

		Ref<Model> model = new Model();

		std::string pathStr(path.data());

		int flag =
			aiProcess_Triangulate |
			aiProcess_PreTransformVertices |
			aiProcess_CalcTangentSpace |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_OptimizeMeshes;

		Assimp::Importer importer;
		auto scene = importer.ReadFile(pathStr,flag);

		if (scene == nullptr) {
			LOG_ERROR("Failed to load {}",path);
			return nullptr;
		}

		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			LOG_WARNING("Assimp scene is incomplete [{}]", path);
		}

		for (auto mesh : Span<aiMesh*>(scene->mMeshes, scene->mNumMeshes)) {

			if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
				auto getPrimitiveName = [](int type) {
					String message;
					if (type & aiPrimitiveType_POINT) message += "POINT,";
					if (type & aiPrimitiveType_LINE) message += "LINE,";
					if (type & aiPrimitiveType_TRIANGLE) message += "TRIANGLE,";
					if (type & aiPrimitiveType_POLYGON) message += "POLYGON,";
					if (type & aiPrimitiveType_NGONEncodingFlag) message += "NGON,";
					if (!message.empty()) message.pop_back();
					return message;
					};
				LOG_ERROR("非対応のプリミティブライプです [{}]", getPrimitiveName(mesh->mPrimitiveTypes));
				//continue;
			}

			graphics::MeshData meshData;
			meshData.name = mesh->mName.C_Str();

			// 頂点
			if (mesh->mVertices) {
				meshData.positions.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mVertices, mesh->mNumVertices)) {
					meshData.positions.emplace_back(v.x, v.y, v.z);
				}
			}
			if (mesh->mNormals) {
				meshData.colors.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mNormals, mesh->mNumVertices)) {
					meshData.normals.emplace_back(v.x, v.y, v.z);
				}
			}
			if (mesh->mTangents) {
				meshData.tangents.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mTangents, mesh->mNumVertices)) {
					meshData.tangents.emplace_back(v.x, v.y, v.z, 1.0f);
				}
			}
			if (mesh->mColors[0]) {
				meshData.colors.reserve(mesh->mNumVertices);
				for (auto c : Span<aiColor4D>(mesh->mColors[0], mesh->mNumVertices)) {
					meshData.colors.emplace_back(c.r, c.g, c.b, c.a);
				}
			}
			if (mesh->mTextureCoords[0]) {
				meshData.uvs.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[0], mesh->mNumVertices)) {
					meshData.uvs.emplace_back(v.x, 1 - v.y);
				}
			}
			if (mesh->mTextureCoords[1]) {
				meshData.uvs1.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[1], mesh->mNumVertices)) {
					meshData.uvs1.emplace_back(v.x, 1 - v.y);
				}
			}
			if (mesh->mTextureCoords[2]) {
				meshData.uvs2.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[2], mesh->mNumVertices)) {
					meshData.uvs2.emplace_back(v.x, 1 - v.y);
				}
			}
			if (mesh->mTextureCoords[3]) {
				meshData.uvs3.reserve(mesh->mNumVertices);
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[3], mesh->mNumVertices)) {
					meshData.uvs3.emplace_back(v.x, 1 - v.y);
				}
			}

			// インデックス
			meshData.indices.reserve(mesh->mNumFaces * 3);
			for (auto face : Span<aiFace>(mesh->mFaces, mesh->mNumFaces)) {
				if (face.mNumIndices == 3) {
					meshData.indices.emplace_back(face.mIndices[0]);
					meshData.indices.emplace_back(face.mIndices[1]);
					meshData.indices.emplace_back(face.mIndices[2]);
				}
			}
			meshData.indices.shrink_to_fit();

			// サブメッシュ
			auto& submesh = meshData.submeshes.emplace_back();
			submesh.indexCount = meshData.indices.size();

			model->m_mesh = graphics::Mesh::Create(std::move(meshData));

			break;
		}

		// テクスチャ
		Map<String,Ref<rhi::Texture>,std::less<>> textures;
		//for(auto t : Span<aiTexture*>(scene->mTextures, scene->mNumTextures)) {
		//	
		//	String name(t->mFilename.data, t->mFilename.length);
		//
		//	Ref<rhi::Texture> texture;
		//
		//	// ファイルから読み込み
		//	if (!name.empty()) {
		//		texture = rhi::Texture::Load(name);
		//	}
		//
		//	// 埋め込みデータから読み込み
		//	if (!texture && t->pcData) {
		//		bool bCompressed = t->mHeight == 0;
		//		if (bCompressed) {
		//			BlobView blob(t->pcData, t->mWidth);
		//			texture = rhi::Texture::Create("name",blob);
		//		} else {
		//			Bitmap bitmap(t->mWidth, t->mHeight, IntColor::Black);
		//			memcpy_s(bitmap.data(), bitmap.byteSize(), t->pcData, t->mWidth* t->mHeight * sizeof(aiTexel));
		//			texture = rhi::Texture::Create("name", bitmap);
		//		}
		//	}
		//
		//	if (texture) {
		//		textures[name] = texture;
		//	}
		//}

		auto code = File::ReadAllText("Asset/Shader/GraphicTest.hlsl");
		OB_ASSERT(code, "ファイル読み込み失敗");
		auto vs = rhi::Shader::CompileVS(*code);
		auto ps = rhi::Shader::CompilePS(*code);

		// マテリアル
		for (auto m : Span<aiMaterial*>(scene->mMaterials, scene->mNumMaterials)) {

			String name(m->GetName().C_Str());

			// マテリアル生成
			using namespace ob::graphics;
			auto material = [&] {			
				MaterialDesc desc;
				desc.name = name;
				desc.matrixProperties = { "Matrix" };
				desc.textureProperties = { "Main" };
			
				MaterialPass& opaque = desc.passes["Opaque"];
				opaque.depthStencil.depth.enable = true;
				opaque.colors = { TextureFormat::RGBA8 ,TextureFormat::RGBA8 ,TextureFormat::RGBA8 };	// Shaderに情報を持たせたい
				opaque.depth = TextureFormat::D32;
				opaque.vs = vs;
				opaque.ps = ps;
				opaque.requiredLayout = {
					{Semantic::Position,ElementType::Float,4},
					{Semantic::Normal,ElementType::Float,4},
					{Semantic::TexCoord,ElementType::Float,2},
				};
			
				return Material::Create(desc);
			}();
			material->setMatrix("Matrix", Matrix::Identity);

			// テクスチャ読み込み
			Ref<rhi::Texture> texture = rhi::Texture::Black();
			if (aiString mainTexPath; m->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), mainTexPath) == AI_SUCCESS) {
				auto fullPath = Path::Combine(directory, mainTexPath.C_Str());
				if (File::Exists(fullPath)) {
					texture = textures[fullPath] = rhi::Texture::Load(fullPath);
				}
			}
			if (!texture) texture = rhi::Texture::Black();
			material->setTexture("Main", texture);

			if (aiColor4D color; m->Get(AI_MATKEY_COLOR_DIFFUSE,color) == AI_SUCCESS) {
				material->setColor("Main", Color(color.r, color.g, color.b,color.a));
			}

			model->m_materialMap[name] = model->m_materials.size();
			model->m_materials.push_back(material);
		}

		// アニメーション

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
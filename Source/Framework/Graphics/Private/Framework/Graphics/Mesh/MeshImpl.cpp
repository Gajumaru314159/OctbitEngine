//***********************************************************
//! @file
//! @brief		メッシュ実装
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Mesh/MeshImpl.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/Graphics/Material/MaterialManager.h>


#include <Framework/Core/String/StringEncoder.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Mesh/MeshData.h>
#include <Framework/Graphics/Material/Material.h>

namespace ob::graphics {

	Ref<Mesh> Mesh::Load(StringView path) {

		std::string pathStr(path.data());

		Assimp::Importer importer;
		importer.ReadFile(pathStr, 0);
		auto scene = importer.ApplyPostProcessing(aiProcess_Triangulate);
		
		if (scene == nullptr) {
			LOG_ERROR("Failed to load {}", path);
			return nullptr;
		}

		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			LOG_WARNING("Assimp scene is incomplete [{}]",path);
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
					if(!message.empty()) message.pop_back();
					return message;
				};
				LOG_ERROR("非対応のプリミティブライプです [{}]", getPrimitiveName(mesh->mPrimitiveTypes));
				//continue;
			}

			graphics::MeshData meshData;
			meshData.name = Format("{} ({})", path,mesh->mName.C_Str());

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
					meshData.tangents.emplace_back(v.x, v.y, v.z, 1.0f);
				}
			}
			if (mesh->mColors[0]) {
				for (auto c : Span<aiColor4D>(mesh->mColors[0], mesh->mNumVertices)) {
					meshData.colors.emplace_back(c.r, c.g, c.b, c.a);
				}
			}
			if (mesh->mTextureCoords[0]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[0], mesh->mNumVertices)) {
					meshData.uvs.emplace_back(v.x, 1 - v.y);
				}
			}
			if (mesh->mTextureCoords[1]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[1], mesh->mNumVertices)) {
					meshData.uvs1.emplace_back(v.x, 1 - v.y);
				}
			}
			if (mesh->mTextureCoords[2]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[2], mesh->mNumVertices)) {
					meshData.uvs2.emplace_back(v.x, 1 - v.y);
				}
			}
			if (mesh->mTextureCoords[3]) {
				for (auto v : Span<aiVector3D>(mesh->mTextureCoords[3], mesh->mNumVertices)) {
					meshData.uvs3.emplace_back(v.x, 1 - v.y);
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

			// サブメッシュ
			auto& submesh = meshData.submeshes.emplace_back();
			submesh.indexCount = meshData.indices.size();

			return graphics::Mesh::Create(std::move(meshData));
		}

		return nullptr;
	}

}


namespace ob::graphics {

	std::atomic<s64> MeshImpl::s_id = 0;

	//!	@brief			生成
	Ref<Mesh> Mesh::Create(const MeshData& meshData) {
		return new MeshImpl(meshData);
	}
	//!	@brief			生成
	Ref<Mesh> Mesh::Create(MeshData&& meshData) {
		return new MeshImpl(meshData);
	}


	//!	@brief			コンストラクタ
	MeshImpl::MeshImpl(const MeshData& meshData) {

		m_id = s_id.fetch_add(1);

		if (auto manager = MaterialManager::Get()) {
			m_layoutId = manager->getVertexLayoutId(m_layout);
		} else {
			LOG_ERROR("MaterialManagerが未初期化です");
			//return;
		}

		m_initByMeshData = true;
		m_meshData = meshData;
		initLayoutFromMeshData(meshData);

	}

	//!	@brief			コンストラクタ
	MeshImpl::MeshImpl(MeshData&& meshData) {

		m_id = s_id.fetch_add(1);
		
		if (auto manager = MaterialManager::Get()) {
			m_layoutId = manager->getVertexLayoutId(m_layout);
		} else {
			LOG_ERROR("MaterialManagerが未初期化です");
			return;
		}

		m_initByMeshData = true;
		m_meshData = std::move(meshData);
		initLayoutFromMeshData(m_meshData);

		if (auto manager = MaterialManager::Get()) {

		}

	}

	//!	@brief			頂点レイアウトIDを取得
	VertexLayoutId MeshImpl::getVertexLayoutId()const {
		return m_layoutId;
	}

	//!	@brief			MeshDataから頂点レイアウトを再設定
	void MeshImpl::initLayoutFromMeshData(const MeshData& meshData) {

		using namespace ob::rhi;

		size_t offset = 0;
		m_layout = {};

		if (meshData.positions.empty()) {
			LOG_ERROR("Meshの生成に失敗。MeshDataにpositionsが含まれていません。");
			return;
		}


#define ADD_VERTEX_ATTRIBUTE(type,container,semantic,...)\
		if (!meshData.container.empty()) {\
			update_max(vertexCount, meshData.container.size());\
			m_layout.attributes.emplace_back(Semantic::semantic, (s32)offset, __VA_ARGS__);\
			offset += sizeof(type);\
		}

		size_t vertexCount = 0;

		ADD_VERTEX_ATTRIBUTE(Vec4, positions, Position, ElementType::Float, 4);

		if (!meshData.colors.empty()) {
			ADD_VERTEX_ATTRIBUTE(Color, colors, Color, ElementType::Float, 4);
		} else if (!meshData.intColors.empty()) {
			offset -= sizeof(f32);
			ADD_VERTEX_ATTRIBUTE(IntColor, intColors, Color, ElementType::UInt32, 1);
		}

		ADD_VERTEX_ATTRIBUTE(Vec4, normals	, Normal	, ElementType::Float, 4);
		ADD_VERTEX_ATTRIBUTE(Vec4, tangents	, Tangent	, ElementType::Float, 4);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs		, TexCoord	, ElementType::Float, 2, 0);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs1		, TexCoord	, ElementType::Float, 2, 1);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs2		, TexCoord	, ElementType::Float, 2, 2);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs3		, TexCoord	, ElementType::Float, 2, 3);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs4		, TexCoord	, ElementType::Float, 2, 4);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs5		, TexCoord	, ElementType::Float, 2, 5);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs6		, TexCoord	, ElementType::Float, 2, 6);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs7		, TexCoord	, ElementType::Float, 2, 7);

		auto stribe = align_up(offset, 16);

		// 頂点バッファ生成
		{
			auto desc = BufferDesc::Vertex<s32>(0);
			desc.name = meshData.name;
			desc.size = stribe * vertexCount;
			desc.stride = stribe;

			m_vertexBuffer = Buffer::Create(desc);
		}

		// インデックスバッファ生成
		{
			size_t indexStribe = 0;
			size_t indexCount = 0;
			if (!meshData.indices.empty()) {
				indexStribe = sizeof(u16);
				indexCount = meshData.indices.size();
			} else if (!meshData.indices32.empty()) {
				indexStribe = sizeof(u32);
				indexCount = meshData.indices32.size();
			}

			if (0 < indexStribe) {

				auto desc = BufferDesc::Index<s32>(0);
				desc.name = meshData.name;
				desc.size = indexStribe * indexCount;
				desc.stride = indexStribe;

				m_indexBuffer = Buffer::Create(desc);

			}
		}

		// 頂点バッファ更新
		if (m_vertexBuffer) {
			m_vertexBuffer->update(
				[&meshData, stribe, vertexCount](void* ptr) {

					size_t offset = 0;

#define				COPY_VERTEX_ELEMENT(container)\
					{\
						using type = std::remove_const_t<decltype(meshData.container)::value_type>;\
						if (!meshData.container.empty()) {\
							for (size_t i = 0; i < vertexCount; ++i) {\
								*GetOffsetPtr<type>(ptr, i * stribe + offset) = meshData.container[i];\
							}\
							offset+=sizeof(type);\
						}\
					}

					COPY_VERTEX_ELEMENT(positions);

					if (!meshData.colors.empty()) {
						offset += sizeof(f32);
						COPY_VERTEX_ELEMENT(colors);
					} else if (!meshData.intColors.empty()) {
						COPY_VERTEX_ELEMENT(intColors);
					} else {
						offset += sizeof(f32);
					}

					COPY_VERTEX_ELEMENT(normals);
					if (!meshData.normals.empty()) {
						offset += sizeof(f32);
					}

					COPY_VERTEX_ELEMENT(tangents);
					COPY_VERTEX_ELEMENT(uvs);
					COPY_VERTEX_ELEMENT(uvs1);
					COPY_VERTEX_ELEMENT(uvs2);
					COPY_VERTEX_ELEMENT(uvs3);
					COPY_VERTEX_ELEMENT(uvs4);
					COPY_VERTEX_ELEMENT(uvs5);
					COPY_VERTEX_ELEMENT(uvs6);
					COPY_VERTEX_ELEMENT(uvs7);

				}
			);
		}

		// インデックスバッファ更新
		if (m_indexBuffer) {
			m_indexBuffer->update(
				[&meshData](void* ptr) {
					if (!meshData.indices.empty()) {
						size_t size = sizeof(u16) * meshData.indices.size();
						memcpy_s(ptr, size, meshData.indices.data(), size);
						return;
					}
					if (!meshData.indices32.empty()) {
						size_t size = sizeof(u32) * meshData.indices32.size();
						memcpy_s(ptr, size, meshData.indices32.data(), size);
						return;
					}
				}
			);

		}

	}

	//!	@brief			頂点レイアウトを設定
	//! @details		setVertices で設定されるBlobの解釈方法を設定します。
	const Mesh::VertexLayout& MeshImpl::getVertexLayout()const {
		return m_layout;
	}

	//!	@brief			メッシュデータを持っているか
	bool MeshImpl::hasMeshData()const {
		return m_initByMeshData;
	}

	//!	@brief			メッシュデータを取得
	//! @details		```hasMeshData() == false```の場合空のMeshDataを返す。
	const MeshData& MeshImpl::getMeshData()const {
		return m_meshData;
	}

	//!	@brief			サブメッシュの数を取得
	s32 MeshImpl::getSubMeshCount()const {
		return m_meshData.submeshes.size();
	}

	//!	@brief			指定したインデックスのサブメッシュを取得
	SubMesh MeshImpl::getSubMesh(s32 index)const {
		if (is_in_range(index,m_meshData.submeshes)) {
			return m_meshData.submeshes[index];
		}
		return {};
	}

	//!	@brief			サブメッシュのリストを取得
	const Vector<SubMesh>& MeshImpl::getSubMeshes()const {
		return m_meshData.submeshes;
	}

	//!	@brief			描画コマンドを記録
	void MeshImpl::record(Ref<rhi::CommandList>& cmdList, s32 submeshIndex) {

		if (m_indexBuffer) {
			cmdList->setIndexBuffer(m_indexBuffer);
		}
		if (m_vertexBuffer) {
			
			auto submesh = getSubMesh(submeshIndex);

			rhi::DrawIndexedParam param{};
			param.startVertex = submesh.baseVertex;
			param.startIndex = submesh.indexStart;
			param.indexCount = submesh.indexCount;

			cmdList->setVertexBuffer(m_vertexBuffer);
			cmdList->drawIndexed(param);

		}
	}


}
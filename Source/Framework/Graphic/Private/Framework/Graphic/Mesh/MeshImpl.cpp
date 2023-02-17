//***********************************************************
//! @file
//! @brief		メッシュ実装
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Mesh/MeshImpl.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphic {


	//@―---------------------------------------------------------------------------
	//!	@brief			生成
	//@―---------------------------------------------------------------------------
	Ref<Mesh> Mesh::Create(const MeshData& meshData) {
		return new MeshImpl(meshData);
	}
	//@―---------------------------------------------------------------------------
	//!	@brief			生成
	//@―---------------------------------------------------------------------------
	Ref<Mesh> Mesh::Create(MeshData&& meshData) {
		return new MeshImpl(meshData);
	}


	//@―---------------------------------------------------------------------------
	//!	@brief			コンストラクタ
	//@―---------------------------------------------------------------------------
	MeshImpl::MeshImpl(const MeshData& meshData) {
		m_initByMeshData = true;
		m_meshData = meshData;
		initLayoutFromMeshData(meshData);
	}

	MeshImpl::MeshImpl(MeshData&& meshData) {
		m_initByMeshData = true;
		m_meshData = std::move(meshData);
		initLayoutFromMeshData(meshData);
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			MeshDataから頂点レイアウトを再設定
	//@―---------------------------------------------------------------------------
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
			m_layout.attributes.emplace_back(Semantic::semantic, offset, __VA_ARGS__);\
			offset += sizeof(type);\
		}

		size_t vertexCount = 0;

		ADD_VERTEX_ATTRIBUTE(Vec3, positions, Position, Type::Float, 3);

		if (!meshData.colors.empty()) {
			offset += sizeof(f32);
			ADD_VERTEX_ATTRIBUTE(Color, colors, Color, Type::Float, 4);
		} else if (!meshData.intColors.empty()) {
			ADD_VERTEX_ATTRIBUTE(IntColor, intColors, Color, Type::UInt32, 1);
		} else {
			offset += sizeof(f32);
		}

		ADD_VERTEX_ATTRIBUTE(Vec4, normals	, Normal	, Type::Float, 3);
		ADD_VERTEX_ATTRIBUTE(Vec4, tangents	, Tangent	, Type::Float, 4);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs		, TexCoord	, Type::Float, 2, 0);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs1		, TexCoord	, Type::Float, 2, 1);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs2		, TexCoord	, Type::Float, 2, 2);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs3		, TexCoord	, Type::Float, 2, 3);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs4		, TexCoord	, Type::Float, 2, 4);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs5		, TexCoord	, Type::Float, 2, 5);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs6		, TexCoord	, Type::Float, 2, 6);
		ADD_VERTEX_ATTRIBUTE(Vec2, uvs7		, TexCoord	, Type::Float, 2, 7);

		auto stribe = align_up(offset, 16);

		// 頂点バッファ生成
		{
			auto desc = BufferDesc::Vertex<s32>(0);
			desc.name = meshData.name;
			desc.bufferSize = stribe * vertexCount;
			desc.bufferStride = stribe;

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
				desc.bufferSize = indexStribe * indexCount;
				desc.bufferStride = indexStribe;

				m_indexBuffer = Buffer::Create(desc);

			}
		}

		// 頂点バッファ更新
		if (m_vertexBuffer) {
			m_vertexBuffer->update(
				[&meshData, stribe, vertexCount](void* ptr) {

					size_t offset = 0;

#define				COPY_VERTEX_ELEMENT(type,container)\
					static_assert(std::is_same_v < type, decltype(meshData.container)::value_type>);\
					if (!meshData.container.empty()) {\
						for (size_t i = 0; i < vertexCount; ++i) {\
							*GetOffsetPtr<type>(ptr, i * stribe + offset) = meshData.container[i];\
						}\
						offset+=sizeof(type);\
					}

					COPY_VERTEX_ELEMENT(Vec3, positions);

					if (!meshData.colors.empty()) {
						offset += sizeof(f32);
						COPY_VERTEX_ELEMENT(Color, colors);
					} else if (!meshData.intColors.empty()) {
						COPY_VERTEX_ELEMENT(IntColor, intColors);
					} else {
						offset += sizeof(f32);
					}

					COPY_VERTEX_ELEMENT(Vec3, normals, Normal, Type::Float, 3);
					if (!meshData.normals.empty()) {
						offset += sizeof(f32);
					}

					COPY_VERTEX_ELEMENT(Vec4, tangents);
					COPY_VERTEX_ELEMENT(Vec2, uvs);
					COPY_VERTEX_ELEMENT(Vec2, uvs1);
					COPY_VERTEX_ELEMENT(Vec2, uvs2);
					COPY_VERTEX_ELEMENT(Vec2, uvs3);
					COPY_VERTEX_ELEMENT(Vec2, uvs4);
					COPY_VERTEX_ELEMENT(Vec2, uvs5);
					COPY_VERTEX_ELEMENT(Vec2, uvs6);
					COPY_VERTEX_ELEMENT(Vec2, uvs7);

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

	//@―---------------------------------------------------------------------------
	//!	@brief			頂点レイアウトを設定
	//! @details		setVertices で設定されるBlobの解釈方法を設定します。
	//@―---------------------------------------------------------------------------
	const Mesh::VertexLayout& MeshImpl::getVertexLayout()const {
		return m_layout;
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			メッシュデータを持っているか
	//@―---------------------------------------------------------------------------
	bool MeshImpl::hasMeshData()const {
		return m_initByMeshData;
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			メッシュデータを取得
	//! @details		```hasMeshData() == false```の場合空のMeshDataを返す。
	//@―---------------------------------------------------------------------------
	const MeshData& MeshImpl::getMeshData()const {
		return m_meshData;
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			サブメッシュの数を取得
	//@―---------------------------------------------------------------------------
	s32 MeshImpl::getSubMeshCount()const {
		return m_meshData.submeshes.size();
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			指定したインデックスのサブメッシュを取得
	//@―---------------------------------------------------------------------------
	SubMesh MeshImpl::getSubMesh(s32 index)const {
		if (is_in_range(index,m_meshData.submeshes)) {
			return m_meshData.submeshes[index];
		}
		return {};
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			サブメッシュのリストを取得
	//@―---------------------------------------------------------------------------
	const Array<SubMesh>& MeshImpl::getSubMeshes()const {
		return m_meshData.submeshes;
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			描画コマンドを記録
	//@―---------------------------------------------------------------------------
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


}// namespace ob
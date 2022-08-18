//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/MeshData.h>
#include <Framework/Graphic/Buffer.h>

namespace ob::graphic {
	//struct MeshData;
}


namespace ob::rendering {

	class Mesh {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  デフォルトコンストラクタ
		//@―---------------------------------------------------------------------------
		Mesh() = default;

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ(MeshDataから)
		//@―---------------------------------------------------------------------------
		template<typename TVertex, typename TIndex = u16>
		Mesh(const ob::graphic::MeshData<TVertex, TIndex>& meshData)
			: Mesh(BlobView(meshData.vertices), sizeof(TVertex), BlobView(meshData.indices), sizeof(TIndex))
		{
		}

		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator = (const Mesh&) = default;
		Mesh& operator = (Mesh&&) = default;


		size_t vertexCount()const;
		size_t indexCount()const;

	private:

		Mesh(BlobView vertices, size_t vertexSize, BlobView indices, size_t indexSize);

	private:
		graphic::Buffer m_indexBuffer;
		graphic::Buffer m_vertexBuffer;
		size_t m_vertexCount = 0;
		size_t m_indexCount = 0;

	};





	//@―---------------------------------------------------------------------------
	//! @brief  頂点数
	//@―---------------------------------------------------------------------------
	inline size_t Mesh::vertexCount()const {
		return m_vertexCount;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  インデックス数
	//@―---------------------------------------------------------------------------
	inline size_t Mesh::indexCount()const {
		return m_indexCount;
	}

	/*
	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Mesh {
	public:
		using MeshData = ob::graphic::MeshData;
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		Mesh();
		explicit Mesh(const MeshData& meshData);

		size_t num_vertices()const;
		size_t num_triangles()const;
		Box boundingBox()const;
		Sphere boundingSphere()const;

		void draw()const;

		// Transform / Color / Subset / Instancing

	private:

		SPtr<class MeshImpl> m_impl;

	};
	*/

}// namespcae ob
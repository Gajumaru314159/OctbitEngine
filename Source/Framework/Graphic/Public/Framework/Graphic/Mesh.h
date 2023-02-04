//***********************************************************
//! @file
//! @brief		メッシュバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/MeshData.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  メッシュバッファ
	//@―---------------------------------------------------------------------------
	class Mesh {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		template<typename TVertex, typename TIndex>
		Mesh(const rhi::MeshData<TVertex, TIndex>& mesh,StringView name);


		//@―---------------------------------------------------------------------------
		//! @brief  頂点バッファを取得
		//@―---------------------------------------------------------------------------
		const Ref<rhi::Buffer>& getVertexBuffer()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  インデックスバッファを取得
		//@―---------------------------------------------------------------------------
		const Ref<rhi::Buffer>& getIndexBuffer()const noexcept;

		size_t getVertexCount()const noexcept { return m_vertexCount; }
		size_t getIndexCount()const noexcept { return m_indexCount; }

	private:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		Mesh(const void* pVertexData, size_t vertexStribe, size_t vertexCount, const void* pIndexData, size_t indexStribe, size_t indexCount,StringView name);


	private:

		Ref<rhi::Buffer> m_vertexBuffer;
		Ref<rhi::Buffer> m_indexBuffer;
		size_t m_vertexCount;
		size_t m_indexCount;

	};


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline Mesh::Mesh(const rhi::MeshData<TVertex, TIndex>& mesh,StringView name)
		: Mesh(mesh.vertices.data(), sizeof(TVertex), mesh.vertices.size(), mesh.indices.data(), sizeof(TIndex), mesh.indices.size(),name)
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点バッファを取得
	//@―---------------------------------------------------------------------------
	inline const Ref<rhi::Buffer>& Mesh::getVertexBuffer()const noexcept {
		return m_vertexBuffer;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  インデックスバッファを取得
	//@―---------------------------------------------------------------------------
	inline const Ref<rhi::Buffer>& Mesh::getIndexBuffer()const noexcept {
		return m_indexBuffer;
	}

}// namespcae ob
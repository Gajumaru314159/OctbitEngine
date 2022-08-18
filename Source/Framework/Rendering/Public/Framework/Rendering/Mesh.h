﻿//***********************************************************
//! @file
//! @brief		メッシュバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Buffer.h>
#include <Framework/Graphic/MeshData.h>

namespace ob::rendering {

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
		Mesh(const graphic::MeshData<TVertex, TIndex>& mesh);


		//@―---------------------------------------------------------------------------
		//! @brief  名前を設定
		//@―---------------------------------------------------------------------------
		void setName(StringView name);


		//@―---------------------------------------------------------------------------
		//! @brief  頂点バッファを取得
		//@―---------------------------------------------------------------------------
		const graphic::Buffer& getVertexBuffer()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  インデックスバッファを取得
		//@―---------------------------------------------------------------------------
		const graphic::Buffer& getIndexBuffer()const noexcept;


	private:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		Mesh(const void* pVertexData, size_t vertexStribe, size_t vertexCount, const void* pIndexData, size_t indexStribe, size_t indexCount);


	private:

		graphic::Buffer m_vertexBuffer;
		graphic::Buffer m_indexBuffer;

	};


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline Mesh::Mesh(const graphic::MeshData<TVertex, TIndex>& mesh)
		: Mesh(mesh.vertices.data(), sizeof(TVertex), mesh.vertices.size(), mesh.indices.data(), sizeof(TIndex), mesh.indices.size())
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  名前を設定
	//@―---------------------------------------------------------------------------
	inline void Mesh::setName(StringView name) {
		OB_DEBUG_CONTEXT(m_vertexBuffer.setName(Format(TC("{}_Vertex"), name)));
		OB_DEBUG_CONTEXT(m_indexBuffer.setName(Format(TC("{}_Index"), name)));
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点バッファを取得
	//@―---------------------------------------------------------------------------
	inline const graphic::Buffer& Mesh::getVertexBuffer()const noexcept {
		return m_vertexBuffer;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  インデックスバッファを取得
	//@―---------------------------------------------------------------------------
	inline const graphic::Buffer& Mesh::getIndexBuffer()const noexcept {
		return m_indexBuffer;
	}

}// namespcae ob
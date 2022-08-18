//***********************************************************
//! @file
//! @brief		メッシュデータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/Topology.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  メッシュデータ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex = u16>
	struct MeshData {
	public:

		using this_type = MeshData<TVertex, TIndex>;	//!< 型
		using vertex_type = TVertex;				//!< 頂点型
		using index_type = TIndex;					//!< インデックス型
		using VertexArray = Array<TVertex>;			//!< 頂点列型
		using IndexArray = Array<TIndex>;			//!< インデックス列型

		static_assert(std::is_same_v<TIndex, u16> || std::is_same_v<TIndex, u32>, "Invalid index type.");

	public:

		const Topology	topology;   //!< トポロジー
		VertexArray		vertices;   //!< 頂点列
		IndexArray		indices;    //!< インデックス列

	public:

		//@―---------------------------------------------------------------------------
		//! @brief	コンストラクタ
		//@―---------------------------------------------------------------------------
		MeshData(Topology topology = Topology::TriangleList);


		//@―---------------------------------------------------------------------------
		//! @brief  全ての頂点のバイトサイズを取得
		//@―---------------------------------------------------------------------------
		size_t verticesByteSize()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  全てのインデックスのバイトサイズを取得
		//@―---------------------------------------------------------------------------
		size_t indicesByteSize()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  全ての頂点とインデックスを削除
		//@―---------------------------------------------------------------------------
		void clear();


		//@―---------------------------------------------------------------------------
		//! @brief			事前確保
		//@―---------------------------------------------------------------------------
		void reserve(size_t vertex, size_t index);


		//@―---------------------------------------------------------------------------
		//! @brief			頂点とインデックスを追加
		//@―---------------------------------------------------------------------------
		void append(const TVertex& vertex, TIndex index);


		//@―---------------------------------------------------------------------------
		//! @brief			三角形を追加
		//! 
		//! @details		頂点の追加とインデックスを追加します。
		//!					topology が Topology::TriangleListでない場合スキップされます。
		//@―---------------------------------------------------------------------------
		void append(const TVertex& v0, const TVertex& v1, const TVertex& v2);


		//@―---------------------------------------------------------------------------
		//! @brief			四角形を追加
		//! 
		//! @details		頂点の追加とインデックスを追加します。
		//!					topology が Topology::TriangleListでない場合スキップされます。
		//@―---------------------------------------------------------------------------
		void append(const TVertex& v0, const TVertex& v1, const TVertex& v2, const TVertex& v3);


		//@―---------------------------------------------------------------------------
		//! @brief		メッシュをマージ
		//! 
		//! @param mesh	マージするメッシュ
		//@―---------------------------------------------------------------------------
		void merge(const this_type& mesh);

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond


	//@―---------------------------------------------------------------------------
	//! @brief	コンストラクタ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline MeshData<TVertex, TIndex>::MeshData(Topology topology)
		: topology(topology)
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  全ての頂点のバイトサイズを取得
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline size_t MeshData<TVertex, TIndex>::verticesByteSize()const noexcept {
		return sizeof(TVertex) * vertices.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  全てのインデックスのバイトサイズを取得
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline size_t MeshData<TVertex, TIndex>::indicesByteSize()const noexcept {
		return sizeof(TIndex) * indices.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  全ての頂点とインデックスを削除
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void MeshData<TVertex, TIndex>::clear() {
		vertices.clear();
		indices.clear();
	}


	//@―---------------------------------------------------------------------------
	//! @brief			事前確保
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void MeshData<TVertex, TIndex>::reserve(size_t vertex, size_t index) {
		vertices.reserve(vertex);
		indices.reserve(index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			頂点とインデックスを追加
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void MeshData<TVertex, TIndex>::append(const TVertex& vertex, TIndex index){
		vertices.push_back(vertex);
		indices.push_back(indices.size()+1);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			三角形を追加
	//! 
	//! @param dettails	頂点の追加とインデックスを追加します。
	//!					topology が Topology::TriangleListでない場合スキップされます。
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void MeshData<TVertex, TIndex>::append(const TVertex& v0, const TVertex& v1, const TVertex& v2) {
		const auto base = (TIndex)indices.size();

		switch (topology) {
		case Topology::PointList:
			indices.reserve(indices.size() + 3);
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			break;
		case Topology::LineList:
			indices.reserve(indices.size() + 6);
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			indices.push_back(base + 2);
			indices.push_back(base + 0);
			break;
		case Topology::TriangleList:
			indices.reserve(indices.size() + 3);
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			break;
		default:
			return;
		}

		vertices.reserve(vertices.size() + 3);
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			四角形を追加
	//! 
	//! @param dettails	頂点の追加とインデックスを追加します。
	//!					topology が Topology::TriangleListでない場合スキップされます。
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void MeshData<TVertex, TIndex>::append(const TVertex& v0, const TVertex& v1, const TVertex& v2, const TVertex& v3) {
		const auto base = (TIndex)indices.size();

		switch (topology) {
		case Topology::PointList:
			indices.reserve(indices.size() + 4);
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			indices.push_back(base + 3);
			break;
		case Topology::LineList:
			indices.reserve(indices.size() + 8);
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			indices.push_back(base + 2);
			indices.push_back(base + 3);
			indices.push_back(base + 3);
			indices.push_back(base + 0);
			break;
		case Topology::TriangleList:
			indices.reserve(indices.size() + 3);
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			indices.push_back(base + 1);
			indices.push_back(base + 3);
			indices.push_back(base + 2);
			break;
		default:
			return;
		}

		vertices.reserve(vertices.size() + 4);
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
	}


	//@―---------------------------------------------------------------------------
	//! @brief		メッシュをマージ
	//! 
	//! @param mesh	マージするメッシュ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void MeshData<TVertex, TIndex>::merge(const this_type& mesh) {
		vertices.reserve(vertices.size() + mesh.vertices.size());
		indices.reserve(indices.size() + mesh.indices.size());
		vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
		auto base = indices.size();
		for (auto& i : mesh.indices)indices.push_back(base + i);
	}

	//! @endcond
}// namespcae ob
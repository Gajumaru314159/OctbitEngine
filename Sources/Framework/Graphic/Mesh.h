//***********************************************************
//! @file
//! @brief		メッシュ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/Topology.h>

namespace ob::graphic {


	//@―---------------------------------------------------------------------------
	//! @brief  ジオメトリ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex = u16>
	struct Mesh {
	public:

		using this_type = Mesh<TVertex, TIndex>;		//!< 型
		using vertex_type = TVertex;					//!< 頂点型
		using index_type = TIndex;						//!< インデックス型
		using vertices_type = vector<TVertex>;          //!< 頂点列型
		using indices_type = vector<TIndex>;			//!< インデックス列型

	public:

		Topology        topology;   //!< トポロジー
		vertices_type   vertices;   //!< 頂点列
		indices_type    indices;    //!< インデックス列

	public:

		//@―---------------------------------------------------------------------------
		//! @brief	コンストラクタ
		//@―---------------------------------------------------------------------------
		Mesh(Topology topology = Topology::TriangleList);


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
		//! @brief		メッシュをマージ
		//! 
		//! @param mesh	マージするメッシュ
		//@―---------------------------------------------------------------------------
		void merge(const this_type& mesh);


		//@―---------------------------------------------------------------------------
		//! @brief			事前確保
		//@―---------------------------------------------------------------------------
		void reserve(size_t vertex, size_t index);


		//@―---------------------------------------------------------------------------
		//! @brief			頂点とインデックスを追加
		//@―---------------------------------------------------------------------------
		void append(const vertex_type& vertex, u32 index);


		//@―---------------------------------------------------------------------------
		//! @brief			四角形を追加
		//! 
		//! @details		頂点の追加とインデックスを追加します。
		//!					topology が Topology::TriangleListでない場合スキップされます。
		//@―---------------------------------------------------------------------------
		void appendQuad(const vertex_type& v0, const vertex_type& v1, const vertex_type& v2, const vertex_type& v3);


		/*

		Geometry(VertexLayout layout, Topology topology=Topology::TriangleList)
			:layout(layout),topology(topology){}
		Geometry(VertexLayout layout, vector<TVertex> vertices, Topology topology = Topology::TriangleList)
			:layout(layout), vertices(vertices), topology(topology) {}

		void addTriangle(const vertex_type& v0, const vertex_type& v1, const vertex_type& v2);
		void addQuad(const vertex_type& v0, const vertex_type& v1, const vertex_type& v2, const vertex_type& v3);

		Topology            getTopology()const noexcept;
		const VertexLayout& getLayout()const noexcept;
		size_t              getVertexArrayCount()

		*/
	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond


	//@―---------------------------------------------------------------------------
	//! @brief	コンストラクタ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline Mesh<TVertex, TIndex>::Mesh(Topology topology)
		: topology(topology)
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  全ての頂点のバイトサイズを取得
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline size_t Mesh<TVertex, TIndex>::verticesByteSize()const noexcept {
		return sizeof(TVertex) * vertices.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  全てのインデックスのバイトサイズを取得
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline size_t Mesh<TVertex, TIndex>::indicesByteSize()const noexcept {
		return sizeof(TIndex) * indices.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  全ての頂点とインデックスを削除
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void Mesh<TVertex, TIndex>::clear() {
		vertices.clear();
		indices.clear();
	}


	//@―---------------------------------------------------------------------------
	//! @brief		メッシュをマージ
	//! 
	//! @param mesh	マージするメッシュ
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void Mesh<TVertex, TIndex>::merge(const this_type& mesh) {
		vertices.reserve(vertices.size() + mesh.vertices.size());
		indices.reserve(indices.size() + mesh.indices.size());
		vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
		auto base = indices.size();
		for (auto& i : mesh.indices)indices.push_back(base + i);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			事前確保
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void Mesh<TVertex, TIndex>::reserve(size_t vertex, size_t index) {
		vertices.reserve(vertex);
		indices.reserve(index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			頂点とインデックスを追加
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void Mesh<TVertex, TIndex>::append(const vertex_type& vertex, u32 index){
		vertices.push_back(vertex);
		indices.push_back(indices.size()+1);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			四角形を追加
	//! 
	//! @param dettails	頂点の追加とインデックスを追加します。
	//!					topology が Topology::TriangleListでない場合スキップされます。
	//@―---------------------------------------------------------------------------
	template<typename TVertex, typename TIndex>
	inline void Mesh<TVertex, TIndex>::appendQuad(const vertex_type& v0, const vertex_type& v1, const vertex_type& v2, const vertex_type& v3) {
		if (topology != Topology::TriangleList)return;
		vertices.reserve(vertices.size() + 4);
		indices.reserve(indices.size() + 6);
		auto base = (TIndex)indices.size();
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		indices.push_back(base + 0);
		indices.push_back(base + 1);
		indices.push_back(base + 2);
		indices.push_back(base + 1);
		indices.push_back(base + 3);
		indices.push_back(base + 2);
	}

	//! @endcond
}// namespcae ob
//***********************************************************
//! @file
//! @brief		ジオメトリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/Topology.h>

namespace ob::graphic {
    
    /*
    //@―---------------------------------------------------------------------------
    //! @brief  ジオメトリ
    //@―---------------------------------------------------------------------------
    template<typename TVertex,typename TIndex = u16>
    struct Geometry {
    public:
        using this_type     = Geometry<TVertex,TIndex>; //!< 型
        using vertex_type   = TVertex;                  //!< 頂点型
        using index_type    = TIndex;                   //!< インデックス型
        using vertices_type = vector<TVertex>;          //!< 頂点列型
        using indices_type  = vector<TIndex>;           //!< インデックス列型
    public:
        Topology        topology;   //!< トポロジー
        VertexLayout    layout;     //!< レイアウト
        vertices_type   vertices;   //!< 頂点列
        indices_type    indices;    //!< インデックス列
    public:

        Geometry(VertexLayout layout, Topology topology=Topology::TriangleList)
            :layout(layout),topology(topology){}
        Geometry(VertexLayout layout, vector<TVertex> vertices, Topology topology = Topology::TriangleList)
            :layout(layout), vertices(vertices), topology(topology) {}

        void addTriangle(const vertex_type& v0, const vertex_type& v1, const vertex_type& v2);
        void addQuad(const vertex_type& v0, const vertex_type& v1, const vertex_type& v2, const vertex_type& v3);

        Topology            getTopology()const noexcept;
        const VertexLayout& getLayout()const noexcept;
        size_t              getVertexArrayCount()
    };

    */




    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
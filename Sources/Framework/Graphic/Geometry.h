//***********************************************************
//! @file
//! @brief		ジオメトリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/Topology.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  型
    //! @see    VertexAttribute
    //! @see    VertexLayout
    //@―---------------------------------------------------------------------------
    enum class Type : u32 {
        Int8,
        Int16,
        Int32,
        UInt8,
        UInt16,
        UInt32,
        Float,
    };

    //@―---------------------------------------------------------------------------
    //! @brief  セマンティクス
    //! @see    VertexAttribute
    //! @see    VertexLayout
    //@―---------------------------------------------------------------------------
    enum class Semantic : u32 {
        Position,
        Normal,
        Color,
        TexCoord,
        Binormal,
        BlendIndices,
        BlendWeights,
        PointSize,
        Tangent,
    };

    //@―---------------------------------------------------------------------------
    //! @brief  頂点属性
    //! @see    VertexLayout
    //@―---------------------------------------------------------------------------
    struct VertexAttribute {
        Semantic	semantic;
        Type		type;
        size_t	    dimention;
        size_t      offset;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  頂点レイアウト
    //@―---------------------------------------------------------------------------
    struct VertexLayout {
        size_t size;
        vector<VertexAttribute> attributes;
    };

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






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
//***********************************************************
//! @file
//! @brief		ジオメトリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/Topology.h>

namespace ob::graphic {


    enum class Type : u32 {
        Int8,
        Int16,
        Int32,
        UInt8,
        UInt16,
        UInt32,
        Float,
    };

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

    struct VertexAttribute {
        Semantic	semantic;
        Type		type;
        std::size_t	dimention;
        std::size_t offset;
    };

    struct VertexLayout {
        vector<VertexAttribute> attributes;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ジオメトリ
    //@―---------------------------------------------------------------------------
    class Geometry {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

    private:

        Topology topology;
        vector<f32> vertices;
        vector<u16> indices;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
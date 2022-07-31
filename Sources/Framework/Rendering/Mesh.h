﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {
    struct MeshData;
}


namespace ob::rendering {

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

}// namespcae ob
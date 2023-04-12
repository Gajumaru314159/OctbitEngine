//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/MeshData.h>

namespace ob::model {

    struct Vertex {
        Vec3    position;
        Vec3    normal;
        Vec3    tangent;
        Vec2    uv;
    };

    using MeshData = graphic::MeshData<Vertex>;
    using Mesh32 = graphic::MeshData<Vertex,u32>;

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    struct ModelData {
        
        Array<MeshData> meshes;
        //Array<Material> materials;
        
    };

}
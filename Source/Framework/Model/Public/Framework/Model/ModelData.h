//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Bounding.h>
#include <Framework/RHI/Types/Topology.h>
#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Material/Material.h>

namespace ob::model {

    struct MeshRange {
        s32             start = 0;
        s32             count = 0;
        s32             base = 0;
        rhi::Topology   topology = rhi::Topology::TriangleList;
    };

    struct ModelParts {
        MeshRange   range;
        Bounds      bounds;
        Ref<graphics::Mesh>     mesh;
        Ref<graphics::Material> material;
    };


    struct ModelData {
        Vector<ModelParts>  parts;        // メッシュとマテリアルの組み合わせ
    };

}
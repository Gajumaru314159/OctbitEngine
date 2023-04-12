//***********************************************************
//! @file
//! @brief		シェーダ定義
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダ・ステージ
    //@―---------------------------------------------------------------------------
    enum class ShaderStage :u32{
        Unknown         = 0,

        // グラフィック
        Vertex          = get_bit(0),
        Hull            = get_bit(1),
        Domain          = get_bit(2),
        Geometry        = get_bit(3),
        Pixel           = get_bit(4),

        // コンピュート
        Compute         = get_bit(5),

        // レイトレーシング
        Task            = get_bit(6),
        Mesh            = get_bit(7),
        RayGen          = get_bit(8),
        AnyHit          = get_bit(9),
        ClosestHit      = get_bit(10),
        Miss            = get_bit(11),
        Intersection    = get_bit(12),
        Callable        = get_bit(13),
        Amplification   = get_bit(14),

        All             = ~((u32)(0)),
    };

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダ・ステージ・セット
    //@―---------------------------------------------------------------------------
    using ShaderStages = BitFlags<ShaderStage>;

}
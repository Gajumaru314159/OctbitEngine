//***********************************************************
//! @file
//! @brief		シェーダ・ステージ・フラグ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダ・ステージ・フラグ
    //@―---------------------------------------------------------------------------
    enum class ShaderStageFlag{
        Unknown         = get_bit(0),
        Vertex          = get_bit(1),
        Hull            = get_bit(2),
        Domain          = get_bit(3),
        Geometry        = get_bit(4),
        Pixel           = get_bit(5),
        Compute         = get_bit(6),
        Task            = get_bit(7),
        Mesh            = get_bit(8),
        RayGen          = get_bit(9),
        AnyHit          = get_bit(10),
        ClosestHit      = get_bit(11),
        Miss            = get_bit(12),
        Intersection    = get_bit(13),
        Callable        = get_bit(14),
    };

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダ・ステージ・フラグ・セット
    //@―---------------------------------------------------------------------------
    using ShaderStageFlags = bit_flags<ShaderStageFlag>;

}// namespace pb::graphic
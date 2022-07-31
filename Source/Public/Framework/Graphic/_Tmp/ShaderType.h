//***********************************************************
//! @file
//! @brief		シェーダ・タイプ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Blob.h>
#include <Framework/Graphic/Types/ShaderStage.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダのバイナリセット
    //! 
    //! @details    実行環境によって使用できるシェーダ言語が異なります。
    //!             |DirectX|Vulkan|
    //!             |-------|------|
    //!             |HLSL   |GLSL  |
    //!             シェーダのエントリー関数は固定です。
    //!             |Vertex|Pixel |
    //!             |------|------|
    //!             |VSMain|PSMain|
    //@―---------------------------------------------------------------------------
    struct ShaderBinarySet {
        Blob vs;
        Blob ps;
    };


    //@―---------------------------------------------------------------------------
    //! @brief      シェーダのコードセット
    //! 
    //! @details    実行環境によって使用できるシェーダ言語が異なります。
    //!             |DirectX|Vulkan|
    //!             |-------|------|
    //!             |HLSL   |GLSL  |
    //!             シェーダのエントリー関数は固定です。
    //!             |Vertex|Pixel |
    //!             |------|------|
    //!             |VSMain|PSMain|
    //@―---------------------------------------------------------------------------
    struct ShaderCodeSet {
        String          code;
        ShaderStages    flags;
    };


}// namespace pb::graphic
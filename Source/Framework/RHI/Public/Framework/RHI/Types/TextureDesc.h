﻿//***********************************************************
//! @file
//! @brief		テクスチャ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureFormat.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ・タイプ
    //@―---------------------------------------------------------------------------
    enum class TextureType {
        Texture1D,          //!< 1Dテクスチャ
        Texture2D,          //!< 2Dテクスチャ
        Texture3D,          //!< 3Dテクスチャ
        Cube,               //!< キューブテクスチャ
        //Texture1DArray,          //!< 1Dテクスチャ配列
        //Texture2DArray,          //!< 2Dテクスチャ配列
        //CubeArray,               //!< キューブテクスチャ配列
        RenderTarget,       //!< レンダーターゲット(Texture2D)
        DepthStencil,       //!< デプス・ステンシル(Texture2D)
    };
    // TODO RenderTarget/DepthStencilを別管轄にする

    enum class TextureUsage {
        Sampling,
        ColorAttachment,
        DepthAttachment,
        ResolveAttachment,
        Storage,
        StorageAttomic,
        CPURead,
        CanUpdate,
        CanCopyFrom,
        CanCopyTo,
    };
    using TextureUsages = BitFlags<TextureUsage>;


    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ定義
    //@―---------------------------------------------------------------------------
    struct TextureDesc {
        TextureType     type        = TextureType::Texture2D;   //!< テクスチャタイプ
        TextureFormat   format      = TextureFormat::RGBA8;     //!< テクスチャフォーマット
        Size            size        = { 0,0,0 };                //!< サイズ
        s32             arrayNum    = 0;                        //!< テクスチャ配列の要素数
        s32             mipLevel    = 0;                        //!< ミップ生成レベル
        Color           color;                                  //!< 初期化色
        // TextureUsages usages;
    };

}// namespcae ob::rhi
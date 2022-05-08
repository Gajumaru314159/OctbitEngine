//***********************************************************
//! @file
//! @brief		テクスチャ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ・タイプ
    //@―---------------------------------------------------------------------------
    enum class TextureType {
        Texture1D,          //!< 1Dテクスチャ
        Texture2D,          //!< 2Dテクスチャ
        Texture3D,          //!< 3Dテクスチャ
        Cube,               //!< キューブテクスチャ
        RenderTarget,       //!< レンダーターゲット(Texture2D)
        DepthStencil,       //!< デプス・ステンシル(Texture2D)
    };


    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ定義
    //@―---------------------------------------------------------------------------
    struct TextureDesc {
        Size            size        = { 0,0,0 };                //!< サイズ
        TextureType     type        = TextureType::Texture2D;   //!< テクスチャタイプ
        TextureFormat   format      = TextureFormat::RGBA8;     //!< テクスチャフォーマット
        s32             arrayNum    = 0;                        //!< テクスチャ配列の要素数
        s32             mipLevel    = 0;                        //!< ミップ生成レベル
        Color           color;
    };

}// namespcae ob::graphic
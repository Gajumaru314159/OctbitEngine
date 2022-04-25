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
        RenderTarget,       //!< レンダーターゲット
        DeptthStencil,      //!< デプス・ステンシル
    };


    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ定義
    //@―---------------------------------------------------------------------------
    struct TextureDesc {
    public:
        Size            size = { 0,0,0 };               //!< サイズ
        s32             arrayNum = 0;                   //!< テクスチャ配列の要素数
        s32             mipLevel = 0;                   //!< ミップ生成レベル
        TextureType     type = TextureType::Texture2D;  //!< テクスチャタイプ
        TextureFormat   format = TextureFormat::RGBA8;  //!< テクスチャフォーマット

        // ResourceDescにまとめる？
        //ResourceUsage   usage = ResourceUsage::Default;         //!< 利用法
        //BufferType      bufferType = BufferType::RenderTarget;  //!< リソース・タイプ
        //BindFlags       bindFlags;                              //!< バインドフラグ
    };

}// namespcae ob::graphic
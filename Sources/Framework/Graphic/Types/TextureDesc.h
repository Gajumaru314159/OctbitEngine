//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureType.h>
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/ResourceUsage.h>
#include <Framework/Graphic/Types/ResourceType.h>
#include <Framework/Graphic/Types/BindFlag.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    struct TextureDesc {
    public:
        Size            size = { 32,32,32 };            //!< サイズ
        s32             arrayNum = 1;                   //!< テクスチャ配列の要素数
        s32             mipLevel = 0;                   //!< ミップ生成レベル
        TextureType     type = TextureType::Texture2D;  //!< テクスチャタイプ
        TextureFormat   format = TextureFormat::RGB8;   //!< テクスチャフォーマット

        // ResourceDescにまとめる？
        ResourceUsage   usage = ResourceUsage::Default;             //!< 利用法
        ResourceType    resourceType = ResourceType::RenderTarget;  //!< リソース・タイプ
        BindFlags       bindFlags;                                  //!< バインドフラグ
    };

}// namespcae ob::graphic
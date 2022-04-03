//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureType.h>
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/FilterMode.h>
#include <Framework/Graphic/Types/TextureWrapMode.h>
//#include <Framework/Graphic/Types/BindFlag.h>
//#include <Framework/Graphic/Types/ResourceUsage.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ定義
    //@―---------------------------------------------------------------------------
    struct TextureDesc {
    public:
        Size            size = { 0,0,0 };               //!< サイズ
        s32             arrayNum = 0;                   //!< テクスチャ配列の要素数
        s32             mipLevel = 0;                   //!< ミップ生成レベル
        TextureType     type = TextureType::Texture2D;  //!< テクスチャタイプ
        TextureFormat   format = TextureFormat::RGB8;   //!< テクスチャフォーマット

        // ResourceDescにまとめる？
        //ResourceUsage   usage = ResourceUsage::Default;         //!< 利用法
        //BufferType      bufferType = BufferType::RenderTarget;  //!< リソース・タイプ
        //BindFlags       bindFlags;                              //!< バインドフラグ
    };

}// namespcae ob::graphic
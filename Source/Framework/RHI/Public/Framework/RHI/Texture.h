﻿//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Utility/GraphicObjectHolder.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/File/Path.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      スワップチェーン
    //! 
    //! @details    ウィンドウの情報は platform::Window で制御されますが、このクラスでは
    //!             グラフィック機能と結びつけるために必要な情報を制御します。
    //@―---------------------------------------------------------------------------
    class Texture {
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(Texture);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        Texture() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc テクスチャ定義
        //@―---------------------------------------------------------------------------
        Texture(const TextureDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc テクスチャ定義
        //@―---------------------------------------------------------------------------
        Texture(BlobView blob);


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      定義を取得
        //@―---------------------------------------------------------------------------
        auto desc()const->const TextureDesc&;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・サイズを取得
        //@―---------------------------------------------------------------------------
        Size size()const;


        //@―---------------------------------------------------------------------------
        //! @brief      幅を取得
        //@―---------------------------------------------------------------------------
        s32 width()const;


        //@―---------------------------------------------------------------------------
        //! @brief      高さを取得
        //@―---------------------------------------------------------------------------
        s32 height()const;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        auto format()const->TextureFormat;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップレベルを取得
        //@―---------------------------------------------------------------------------
        s32 mipLevels()const;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャタイプを取得
        //@―---------------------------------------------------------------------------
        auto type()const->TextureType;

    };

}// namespace pb::rhi
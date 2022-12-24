﻿//***********************************************************
//! @file
//! @brief		テクスチャ・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/TextureDesc.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・インターフェイス
    //@―---------------------------------------------------------------------------
    class ITexture :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief  サイズ
        //@―---------------------------------------------------------------------------
        virtual Size size()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        virtual TextureFormat format()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップレベルを取得
        //@―---------------------------------------------------------------------------
        virtual s32 mipLevels()const = 0;

    protected:

        virtual ~ITexture() = default;

    };

}// namespace ob::rhi
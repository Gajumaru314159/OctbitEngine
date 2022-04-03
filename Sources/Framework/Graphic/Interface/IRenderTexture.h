﻿//***********************************************************
//! @file
//! @brief		レンダー・テクスチャ・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/ResourceUsage.h>
#include <Framework/Graphic/Texture.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class ITexture;
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダー・テクスチャ・インターフェイス
    //@―---------------------------------------------------------------------------
    class IRenderTexture :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief  テクスチャを取得
        //! 
        //! @param index    マルチレンダーターゲットのインデックス
        //@―---------------------------------------------------------------------------
        virtual const Texture& getTexture(s32 index = 0)const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief  デプス・テクスチャを取得
        //@―---------------------------------------------------------------------------
        virtual const Texture& getDepthStencilTexture()const = 0;

    protected:

        IRenderTexture(StringView name) :GraphicObject(name) {}
        virtual ~IRenderTexture()=default;


    };

}// namespace pb::graphic
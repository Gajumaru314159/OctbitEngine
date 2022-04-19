﻿//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Utility/GraphicObjectHolder.h>
#include <Framework/Graphic/Types/TextureDesc.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      スワップチェーン
    //! 
    //! @details    ウィンドウの情報は platform::Window で制御されますが、このクラスでは
    //!             グラフィック機能と結びつけるために必要な情報を制御します。
    //@―---------------------------------------------------------------------------
    class Texture {
        friend class Device;
        friend class SwapChain;
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
        //! @param name オブジェクト名
        //@―---------------------------------------------------------------------------
        Texture(const TextureDesc& desc, StringView name=TC("Texture"));


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
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        auto format()const->TextureFormat;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップレベルを取得
        //@―---------------------------------------------------------------------------
        s32 mipLevels()const;

        auto type()const->TextureType;




    private:

        class ITexture* m_pImpl;

    };

}// namespace pb::graphic
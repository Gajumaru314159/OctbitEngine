﻿//***********************************************************
//! @file
//! @brief		スワップチェーン・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>
#include <Framework/Graphic/Types/SwapchainDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class ITexture;
    class IRenderTexture;
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      スワップチェーン・インターフェイス
    //! 
    //! @details    モニターに表示するバッファを複数のバッファから切り替えることで
    //!             表示のちらつきを解決する。
    //@―---------------------------------------------------------------------------
    class ISwapChain :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief  定義を取得
        //@―---------------------------------------------------------------------------
        virtual const SwapchainDesc& getDesc()const noexcept = 0;


        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファのサイズを変更
        //@―---------------------------------------------------------------------------
        virtual bool resizeBackBuffer(const Size& size) = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      更新
        //! 
        //! @details    表示するテクスチャを次のバックバッファにします。
        //@―---------------------------------------------------------------------------
        virtual void update(ITexture* pTexture) = 0;


    protected:

        virtual ~ISwapChain() = default;




        // メインディスプレイを初期化
        // 更新
        // 表示
        // ディスプレイ情報を取得
        // 描画フォーマットを取得
        // 同期タイプを取得
        // スワップチェインを取得
        // ウィンドウイベントを追加
        // ウィンドウハンドルを取得
        //  ウィンドウ取得

    };


}// namespace pb::graphic
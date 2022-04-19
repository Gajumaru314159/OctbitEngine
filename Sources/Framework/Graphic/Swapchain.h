﻿//***********************************************************
//! @file
//! @brief		スワップチェーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Utility/GraphicObjectHolder.h>
#include <Framework/Graphic/Types/SwapchainDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class Texture;
}


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
    class OB_API SwapChain:private Noncopyable {
        friend class Device;
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(SwapChain);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        SwapChain() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param  スワップチェーン定義
        //@―---------------------------------------------------------------------------
        explicit SwapChain(const SwapchainDesc& desc, StringView name=TC("SwapChain"));


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファの数を取得
        //@―---------------------------------------------------------------------------
        s32 getBackBufferCount()const;


        //@―---------------------------------------------------------------------------
        //! @brief  VSyncが有効か
        //@―---------------------------------------------------------------------------
        s32 isVSyncEnabled()const;


        //@―---------------------------------------------------------------------------
        //! @brief  HDRが有効か
        //@―---------------------------------------------------------------------------
        s32 isHdrEnabled()const;


        //===============================================================
        // 更新
        //===============================================================
        
        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファのサイズを変更
        //@―---------------------------------------------------------------------------
        bool resizeBackBuffer(const Size& size);


        //@―---------------------------------------------------------------------------
        //! @brief      更新
        //! 
        //! @details    表示するテクスチャを次のバックバッファにします。
        //@―---------------------------------------------------------------------------
        void update(Texture& texture);


    protected:

        class ISwapChain* m_pImpl=nullptr;

    };

}// namespace pb::graphic
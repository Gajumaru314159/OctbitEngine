﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Window.h>
namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウのネイティブ機能アクセサ
    //! 
    //! @details    プラットフォーム固有の機能にアクセスする場合はこのクラスを通して
    //!             アクセスします。
    //@―---------------------------------------------------------------------------
    class WindowNativeAccessor {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  HWND取得
        //@―---------------------------------------------------------------------------
        static void* getHWND(Window& window);

    };

}// namespace ob::platform
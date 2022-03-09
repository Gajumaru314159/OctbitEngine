﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#if defined(OS_WINDOWS)
#include <Windows.h>
#elif defined(OS_LINUX)

#endif

#include "Window.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウのネイティブ機能アクセサ
    //! 
    //! @details    プラットフォーム固有の機能にアクセスする場合はこのクラスを通して
    //!             アクセスします。
    //@―---------------------------------------------------------------------------
    class WindowNativeAccessor {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        static HWND GetHWND(Ref<Window> window);

    private:



    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond

}// namespace ob::platform
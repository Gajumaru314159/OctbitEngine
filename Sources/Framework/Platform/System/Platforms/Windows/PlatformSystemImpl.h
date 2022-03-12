﻿//***********************************************************
//! @file
//! @brief		プラットフォーム・システム実装(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#ifdef OS_WINDOWS
#include "../Base/PlatformSystemImplBase.h"

namespace ob::platform::detail {

    //@―---------------------------------------------------------------------------
    //! @brief  プラットフォーム・システム実装(Windows)
    //@―---------------------------------------------------------------------------
    class PlatformSystemImpl :public PlatformSystemImplBase {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        
        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        PlatformSystemImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~PlatformSystemImpl()override;


        virtual Language getSystemLanguage() const override;

        virtual void addSystemEventListener() override;

    };

}// namespace ob::platform::detail
#endif
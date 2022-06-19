//***********************************************************
//! @file
//! @brief		ウィンドウ(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Type/Language.h>

//============================================
// クラス宣言
//============================================
namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  プラットフォーム・システム
    //@―---------------------------------------------------------------------------
    class PlatformSystem :public Singleton<PlatformSystem> {
    public:

        PlatformSystem();
        ~PlatformSystem();

        //@―---------------------------------------------------------------------------
        //! @brief  プラットフォームのシステム言語を取得する
        //@―---------------------------------------------------------------------------
        Language getSystemLanguage()const;

    private:

        Pimpl<class PlatformSystemImpl> m_impl;

    };

}// namespace ob::platform
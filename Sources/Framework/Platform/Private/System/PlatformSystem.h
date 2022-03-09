//***********************************************************
//! @file
//! @brief		ウィンドウ(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Type/Language.h>

namespace ob::platform {
    //! @cond
    namespace detail {
        class PlatformSystemImpl;
    }
    //! @endcond

    //@―---------------------------------------------------------------------------
    //! @brief  プラットフォーム・システム
    //@―---------------------------------------------------------------------------
    class PlatformSystem :singleton<PlatformSystem> {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  システムを起動する
        //@―---------------------------------------------------------------------------
        bool Startup();

        //@―---------------------------------------------------------------------------
        //! @brief  システムを終了する
        //@―---------------------------------------------------------------------------
        void Shutdown();


        Ref<Window> GetMainWindow();

        Language GetUserLanguage()const;



    private:

        unique_ptr<detail::PlatformSystemImpl> m_implement;

    };

}// namespace ob::platform
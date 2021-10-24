//***********************************************************
//! @file
//! @brief		プラットフォーム・システム実装基底
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Platform/Type/Language.h>
#include <Runtime/Platform/Window/Window.h>

namespace ob {
    namespace platform {
        namespace detail {

            //@―---------------------------------------------------------------------------
            //! @brief  説明
            //@―---------------------------------------------------------------------------
            class PlatformSystemImplBase {
            public:

                //===============================================================
                // コンストラクタ / デストラクタ
                //===============================================================

                //@―---------------------------------------------------------------------------
                //! @brief  説明
                //@―---------------------------------------------------------------------------
                virtual bool Startup() = 0;
                virtual bool Shutdown()=0;

                virtual Language GetLanguage()const=0;

                virtual void AddSystemEventListener()=0;

            public:

                Ref<Window>& GetMainWindow();

            protected:

                void CreateMainWindow();



            protected:

                // イベントマネージャ
                Language m_language;

                Ref<Window> m_mainWindow;

            };

        }// namespace detail
    }// namespace platform
}// namespcae ob
//***********************************************************
//! @file
//! @brief		プラットフォーム・システム実装基底
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Type/Language.h>
#include <Framework/Platform/Window/Window.h>

namespace ob::platform {
    namespace detail {

        //@―---------------------------------------------------------------------------
        //! @brief  プラットフォーム・システム実装基底
        //@―---------------------------------------------------------------------------
        class PlatformSystemImplBase {
        public:

            //===============================================================
            // コンストラクタ / デストラクタ
            //===============================================================
            virtual ~PlatformSystemImplBase() = default;

            virtual Language getSystemLanguage()const = 0;

            virtual void addSystemEventListener() = 0;

        public:

            Ref<Window>& getMainWindow();

        protected:

            void createMainWindow();



        protected:

            // イベントマネージャ
            Language m_language;

            Ref<Window> m_mainWindow;

        };

    }// namespace detail
}// namespace ob::platform
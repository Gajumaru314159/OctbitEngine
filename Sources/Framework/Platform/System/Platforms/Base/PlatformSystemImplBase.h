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

            virtual Language GetLanguage()const = 0;

            virtual void AddSystemEventListener() = 0;

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
}// namespace ob::platform
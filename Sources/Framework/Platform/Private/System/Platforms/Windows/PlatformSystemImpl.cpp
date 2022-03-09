//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "PlatformSystemImpl.h"

#include <Framework/Platform/Window/Implement/Windows/WindowImpl.h>

namespace ob::platform {
    namespace detail {


        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------


        bool PlatformSystemImpl::Startup() {
            LANGID langID = GetSystemDefaultLangID();

            CreateMainWindow();

            return false;
        }

        Language PlatformSystemImpl::GetLanguage() const {
            return Language();
        }

        void PlatformSystemImpl::AddSystemEventListener() {
        }

    }// namespace detail
}// namespace ob::platform

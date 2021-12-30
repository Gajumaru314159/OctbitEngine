//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "WindowNativaAccesseor.h"

#include "Implement/Windows/WindowImpl.h"
#include "Implement/Linux/WindowImpl.h"

namespace ob {
    namespace platform {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        HWND WindowNativeAccessor::GetHWND(Ref<Window> window) {
            if (!window)return NULL;

            auto* pWindowImpl = static_cast<WindowImpl*>(window->m_impl.get());
            return pWindowImpl->GetHWND();
        }

    }// namespace platform
}// namespace ob
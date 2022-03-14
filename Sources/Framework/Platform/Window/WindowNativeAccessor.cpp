//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "WindowNativaAccessor.h"

#include "Implement/Windows/WindowImpl.h"
#include "Implement/Linux/WindowImpl.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  HWNDを取得
    //@―---------------------------------------------------------------------------
    void* WindowNativeAccessor::getHWND(Window& window) {
        auto* pWindowImpl = static_cast<WindowImpl*>(window.m_impl.get());
        return pWindowImpl->getHWND();
    }

}// namespace ob::platform
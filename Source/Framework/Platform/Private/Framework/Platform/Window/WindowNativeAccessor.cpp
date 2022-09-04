//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/WindowNativeAccessor.h>

#include "Implement/Windows/WindowImpl.h"
#include "Implement/Linux/WindowImpl.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  HWNDを取得
    //@―---------------------------------------------------------------------------
    void* WindowNativeAccessor::getHWND(Window& window) {
        auto* pWindowImpl = static_cast<WindowImpl*>(window.m_impl.get());
        if (pWindowImpl == nullptr)return nullptr;
        return pWindowImpl->getHWND();
    }

}// namespace ob::platform
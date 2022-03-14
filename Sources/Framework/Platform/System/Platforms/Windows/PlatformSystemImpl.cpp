//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS
#include "PlatformSystemImpl.h"

#include <Framework/Platform/Window/Implement/Windows/WindowImpl.h>

namespace ob::platform::detail {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    PlatformSystemImpl::PlatformSystemImpl() {
        //createMainWindow();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    PlatformSystemImpl::~PlatformSystemImpl() {
    }

    //@―---------------------------------------------------------------------------
    //! @brief  システムイベントのリスナを追加
    //@―---------------------------------------------------------------------------
    void PlatformSystemImpl::addSystemEventListener() {
    }

}// namespace ob::platform::detail
#endif
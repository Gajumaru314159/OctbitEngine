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
        CreateMainWindow();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    PlatformSystemImpl::~PlatformSystemImpl() {
        int  a = 0;
        a++;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  システムイベントのリスナを追加
    //@―---------------------------------------------------------------------------
    void PlatformSystemImpl::AddSystemEventListener() {
    }

}// namespace ob::platform::detail
#endif
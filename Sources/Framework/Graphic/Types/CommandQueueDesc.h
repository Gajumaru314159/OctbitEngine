//***********************************************************
//! @file
//! @brief		コマンド・キュー定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/CommandListType.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コマンド・キュー定義
    //@―---------------------------------------------------------------------------
    struct CommandQueueDesc {
        CommandListType type;           //!< コマンド・タイプ
        u32             queuesCount;    //!< キューの最大数
    };

}// namespcae ob::graphic
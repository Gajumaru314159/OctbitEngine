//***********************************************************
//! @file
//! @brief		カーネル・タイプ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Platforn/KernelTypePlatform.h"

namespace ob
{
    namespace kernel
    {

        //@―--------------------------------------------------------------------------
        //! @brief スレッド・プライオリティ
        //@―--------------------------------------------------------------------------
        enum class ThreadPriority
        {
            Critical = 0,
            Highest,
            AboveNormal,
            Normal,
            BelowNormal,
            Lowest,
        };

        using ThreadFunc = void(*)(void*);  //!< スレッド関数型
        using FiverFunc = void(*) (void*);  //!< ファイバ関数型

    }// namespace kernel
}// namespcae ob
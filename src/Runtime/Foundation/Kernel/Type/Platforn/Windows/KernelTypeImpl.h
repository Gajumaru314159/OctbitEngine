//***********************************************************
//! @file
//! @brief		カーネル・タイプ(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Windows.h>
//! @cond

namespace ob
{
    namespace kernel
    {

        using ThreadID = DWORD;                           // スレッドID型
        static const ThreadID s_invalidThreadID = 0;      // 無効スレッドID値

    }// namespace kernel
}// namespcae ob

//! @endcond
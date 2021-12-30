//***********************************************************
//! @file
//! @brief		アトミック演算
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Base/Common.h>

namespace ob
{

    struct OB_API AtomicOperator {

        static s32 Load(volatile s32* pSource);
        static s32 Store(volatile s32* pSource,s32 value);
        static s32 Increment(volatile s32* pSource);
        static s32 Decrement(volatile s32* pSource);
        static s32 Add(volatile s32* pSource, s32 amount);
        static s32 Exchange(volatile s32* pSource, s32 exchange);
        static s32 CompareExchange(volatile s32* pSource, s32 exchange, s32 compare);
        static s32 Or(volatile s32* pSource, s32 value);
        static s32 Xor(volatile s32* pSource, s32 value);

    };

}
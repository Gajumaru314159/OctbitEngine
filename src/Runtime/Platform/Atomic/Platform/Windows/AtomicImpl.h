//***********************************************************
//! @file
//! @brief		アトミック演算
//! @author		Gajumaru
//***********************************************************
#pragma once
#if defined(OS_WINDOWS)
#include <Windows.h>

namespace ob
{
    //! @cond
    namespace detail
    {

        bool AtomicCompareAndSwap32(volatile void* pValue, s32 exchange, s32 compare);
        bool AtomicIncrement32(volatile s32& value);
        bool AtomicDecrement32(volatile s32& value);
        bool AtomicAdd32(volatile s32& value, s32 add);
        bool AtomicSub32(volatile s32& value,s32 sub);




        inline bool AtomicCompareAndSwap32(volatile void* pValue, s32 exchange, s32 compare)
        {
            return (::InterlockedCompareExchange((volatile LONG*)pValue, exchange, compare) == compare);
        }
        inline bool AtomicIncrement32(volatile s32& value)
        {
            return ::InterlockedIncrement((volatile LONG*)&value);
        }
        inline bool AtomicDecrement32(volatile s32& value)
        {
            return ::InterlockedDecrement((volatile LONG*)&value);
        }
        inline bool AtomicAdd32(volatile s32& value, s32 add)
        {
            return ::InterlockedExchangeAdd((volatile LONG*)&value,add);
        }
        inline bool AtomicSub32(volatile s32& value, s32 sub)
        {
            return ::InterlockedExchangeAdd((volatile LONG*)&value, sub);
        }

    }
    //! @endcond
}
#endif // OS_WINDOWS
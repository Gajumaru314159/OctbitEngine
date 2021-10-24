//***********************************************************
//! @file
//! @brief		アトミック演算
//! @author		Gajumaru
//! @note       ob::atomicの内部実装に使用します。現在std::atomicを内部的に使用しているため使用していません。
//***********************************************************
#pragma once

namespace ob
{
    //! @cond
    namespace detail
    {

        bool AtomicCompareAndSwap32(volatile void* pValue, s32 exchange, s32 compare);       // CASプリミティブ(32bit)
        bool AtomicIncrement32(volatile s32& value);                                         // インターロック・インクリメント(32bit)
        bool AtomicDecrement32(volatile s32& value);                                         // インターロック・デクリメント(32bit)
        bool AtomicAdd32(volatile s32& value, s32 add);                                      // インターロック・加算(32bit)
        bool AtomicSub32(volatile s32& value, s32 sub);                                      // インターロック・減算(32bit)



        inline bool AtomicCompareAndSwap32(volatile void* pValue, s32 exchange, s32 compare)
        {
            detail::AtomicCompareAndSwap32(pValue, exchange, compare);
        }

        inline bool AtomicIncrement32(volatile s32& value)
        {
            detail::AtomicIncrement32(value);
        }
        inline bool AtomicDecrement32(volatile s32& value)
        {
            detail::AtomicDecrement32(value);
        }
        inline bool AtomicAdd32(volatile s32& value, s32 add)
        {
            detail::AtomicAdd32(value, add);
        }
        inline bool AtomicSub32(volatile s32& value, s32 sub)
        {
            detail::AtomicSub32(value, sub);
        }

    }
    //! @endcond
}
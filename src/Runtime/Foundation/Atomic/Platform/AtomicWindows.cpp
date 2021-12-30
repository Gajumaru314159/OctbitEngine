//***********************************************************
//! @file
//! @brief		アトミック演算
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Foundation/Base/Common.h>
#ifdef OS_WINDOWS
#include "../Atomic.h"
#include <Windows.h>

namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Load(volatile s32* pSource) {
        return CompareExchange(pSource, 0, 0);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Store(volatile s32* pSource,s32 value) {
        return Exchange(pSource, value);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Increment(volatile s32* pSource) {
        return static_cast<s32>(::_InterlockedIncrement((long*)pSource));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Decrement(volatile s32* pSource) {
        return static_cast<s32>(::_InterlockedDecrement((long*)pSource));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Add(volatile s32* pSource, s32 amount) {
        return static_cast<s32>(::_InterlockedExchangeAdd((long*)pSource,amount));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Exchange(volatile s32* pSource, s32 exchange) {
        return static_cast<s32>(::_InterlockedExchange((long*)pSource,exchange));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::CompareExchange(volatile s32* pSource, s32 exchange, s32 compare) {
        return static_cast<s32>(::_InterlockedCompareExchange((long*)pSource,exchange,compare));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Or(volatile s32* pSource, s32 value) {
        return static_cast<s32>(::_InterlockedOr((long*)pSource, value));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    s32 AtomicOperator::Xor(volatile s32* pSource, s32 value) {
        return static_cast<s32>(::_InterlockedXor((long*)pSource, value));
    }

}// namespace ob

#endif// OS_WINDOWS
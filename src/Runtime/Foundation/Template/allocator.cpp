﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "allocator.h"
#include <Runtime/Foundation/Memory/System/MemorySystem.h>

namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    allocator_base::allocator_base() {}

    
    //@―---------------------------------------------------------------------------
    //! @brief メモリ確保
    //@―---------------------------------------------------------------------------
    void* allocator_base::allocate(std::size_t n)
    {
        return MemorySystem::GetHeapAllocator().Allocate(n);
    }


    //@―---------------------------------------------------------------------------
    //! @brief メモリ解放
    //@―---------------------------------------------------------------------------
    void allocator_base::deallocate(void* pBuffer)
    {
        return Allocator::Free(pBuffer);
    }
}// namespace ob
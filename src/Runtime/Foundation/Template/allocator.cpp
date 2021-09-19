//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "allocator.h"
#include <Foundation/Memory/Device/MemoryDevice.h>

namespace ob
{

    allocator_base::allocator_base() {}

    
    //@―---------------------------------------------------------------------------
    //! @brief メモリ確保
    //@―---------------------------------------------------------------------------
    void* allocator_base::allocate(std::size_t n)
    {
        return MemoryDevice::GetHeap().Allocate(n);
    }


    //@―---------------------------------------------------------------------------
    //! @brief メモリ解放
    //@―---------------------------------------------------------------------------
    void allocator_base::deallocate(void* pBuffer)
    {
        return Heap::Free(pBuffer);
    }
}// namespace ob
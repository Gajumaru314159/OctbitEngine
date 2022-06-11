//***********************************************************
//! @file
//! @brief		STLアロケータ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Template/Allocator/STLAllocator.h>
#include <Framework/Core/Memory/System/MemorySystem.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    allocator_base::allocator_base() {}


    //@―---------------------------------------------------------------------------
    //! @brief メモリ確保
    //@―---------------------------------------------------------------------------
    void* allocator_base::allocate(std::size_t n) {
        return MemorySystem::GetHeapAllocator().allocate(n);
    }


    //@―---------------------------------------------------------------------------
    //! @brief メモリ解放
    //@―---------------------------------------------------------------------------
    void allocator_base::deallocate(void* pBuffer) {
        return Allocator::Free(pBuffer);
    }

}// namespace ob::core
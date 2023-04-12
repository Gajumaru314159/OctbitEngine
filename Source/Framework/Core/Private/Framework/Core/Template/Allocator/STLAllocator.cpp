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
    STLAllocatorBase::STLAllocatorBase() {}


    //@―---------------------------------------------------------------------------
    //! @brief メモリ確保
    //@―---------------------------------------------------------------------------
    void* STLAllocatorBase::allocate(std::size_t n) {
        return MemorySystem::GetHeapAllocator().allocate(n);
    }


    //@―---------------------------------------------------------------------------
    //! @brief メモリ解放
    //@―---------------------------------------------------------------------------
    void STLAllocatorBase::deallocate(void* pBuffer) {
        return Allocator::Free(pBuffer);
    }

}
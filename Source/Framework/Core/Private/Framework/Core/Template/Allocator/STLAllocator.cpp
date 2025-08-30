//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <cstdlib>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {

    //! @brief コンストラクタ
    STLAllocatorBase::STLAllocatorBase() {}


    //! @brief メモリ確保
    void* STLAllocatorBase::allocate(std::size_t n) {
        return malloc(n);
    }


    //! @brief メモリ解放
    void STLAllocatorBase::deallocate(void* pBuffer) {
        return free(pBuffer);
    }

}
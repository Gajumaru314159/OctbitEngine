//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Framework/Core/Memory/Allocator.h"
#include <cstdlib>

#ifdef _WIN32
#include <malloc.h>
#endif

namespace ob::core {

    //! @brief malloc/freeベースのアロケーター
    class MallocAllocator : public Allocator {
    public:
        //! @brief メモリを確保する
        void* alloc(size_t size, size_t alignment) override {
#ifdef _WIN32
            return _aligned_malloc(size, alignment);
#else
            return aligned_alloc(alignment, size);
#endif
        }

        //! @brief メモリを解放する
        void free(void* ptr) override {
#ifdef _WIN32
            _aligned_free(ptr);
#else
            ::free(ptr);
#endif
        }
    };

}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Framework/Core/Memory/Allocator.h"
#include "MemoryUtility.h"
#include <atomic>
#include <cstddef>
#include <cassert>

namespace ob::core {

    //! @brief カスタムメモリ確保関数
    //! @param size 確保するサイズ
    //! @param alignment アライメント
    //! @return 確保されたメモリのポインタ
    void* Alloc(size_t size, size_t alignment = alignof(std::max_align_t));

    //! @brief カスタムメモリ解放関数
    //! @param ptr 解放するメモリのポインタ
    void Free(void* ptr);

    //! @brief 現在のメモリ使用量を取得
    //! @return メモリ使用量（バイト）
    size_t GetMemoryUsage();

    namespace internal {

        void InitMemory();

        void FinalizeMemory();

    }

}
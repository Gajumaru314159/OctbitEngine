//***********************************************************
//! @file
//! @brief		メモリ・アロケータ基底クラス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Memory/Allocator/Allocator.h>
#include <Framework/core/Memory/Utility/MemoryUtility.h>
#include <assert.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief		メモリの解放
    //! 
    //! @details	この関数はoperator deleteからも呼び出される。
    //!				解放するポインタは Heap から割り当てられたものである必要がある。
    //@―---------------------------------------------------------------------------
    void Allocator::Free(void* pBuffer) {
        if (pBuffer == nullptr)return;
        auto pHeader = GetOffsetPtr<BlockHeader>(pBuffer, -static_cast<s32>(sizeof(BlockHeader)));

        OB_CHECK_ASSERT(pHeader->pHeap != nullptr,"無効なポインタの開放");
#if defined(OB_DEBUG)
        // メモリの破壊を検知しました。
        OB_CHECK_ASSERT(pHeader->signature == MEMORY_SIGNATURE,"メモリの破壊を検知");
#endif
        pHeader->pHeap->deallocate(pHeader->pRaw);
    }

}// namespace ob::core
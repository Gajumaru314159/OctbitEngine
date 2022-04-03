//***********************************************************
//! @file
//! @brief		メモリ・アロケータ基底クラス
//! @author		Gajumaru
//***********************************************************
#include "Allocator.h"
#include <Framework/Foundation/Memory/Utility/MemoryUtility.h>

namespace ob::foundation {

    //@―---------------------------------------------------------------------------
    //! @brief		メモリの解放
    //! 
    //! @details	この関数はoperator deleteからも呼び出される。
    //!				解放するポインタは Heap から割り当てられたものである必要がある。
    //@―---------------------------------------------------------------------------
    void Allocator::Free(void* pBuffer) {
        if (pBuffer == nullptr)return;
        auto pHeader = GetOffsetPtr<BlockHeader>(pBuffer, -static_cast<s32>(sizeof(BlockHeader)));

        assert(pHeader->pHeap != nullptr);
#if defined(OB_DEBUG)
        // メモリの破壊を検知しました。
        assert(pHeader->signature == MEMORY_SIGNATURE);
#endif
        pHeader->pHeap->deallocate(pHeader->pRaw);
    }

}// namespace ob::foundation
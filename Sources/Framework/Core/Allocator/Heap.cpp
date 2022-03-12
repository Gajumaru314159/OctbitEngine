//***********************************************************
//! @file
//! @brief		ヒープ基底クラス
//! @author		Gajumaru
//***********************************************************
#include "Heap.h"
#include "Platform/PlatformSystemHeap.h"
#include "MimallocHeap.h"
#include "TLSFHeap.h"

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief		メモリの解放
    //! 
    //! @details	この関数はoperator deleteからも呼び出される。
    //!				解放するポインタは Heap から割り当てられたものである必要がある。
    //@―---------------------------------------------------------------------------
    void Heap::Free(void* pBuffer) {
        if (pBuffer == nullptr)return;
        auto pHeader = GetOffsetPtr<HeapHeader>(pBuffer, -static_cast<s32>(sizeof(HeapHeader)));

        assert(pHeader->pHeap != nullptr);
#if defined(OB_DEBUG)
        // メモリの破壊を検知しました。
        assert(pHeader->signature == MEMORY_SIGNATURE);
#endif
        pHeader->pHeap->deallocate(pHeader->pRaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	ヒープの作成
    //@―---------------------------------------------------------------------------
    Heap* Heap::Create(size_t size, HeapMethodType type, const Char* pName, Heap* pParent) {
        Heap* result = nullptr;
        switch (type) {
        case HeapMethodType::System:
            result = new SystemHeap();
            break;
        case HeapMethodType::Mimalloc:
            result = new MimallocHeap(nullptr, 0);
            break;
        case HeapMethodType::TLSF:
            LOG_ERROR_EX("Memory", "not implemented heap mode.");
            break;
        case HeapMethodType::Invalid:
            LOG_ERROR_EX("Memory", "invalid heap mode.");
            break;
        default:
            LOG_ERROR_EX("Memory", "not implemented heap mode.");
            break;
        }

        return result;
    }

}// namespace ob
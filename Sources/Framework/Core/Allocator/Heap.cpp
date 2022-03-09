//***********************************************************
//! @file
//! @brief		ヒープ基底クラス
//! @author		Gajumaru
//***********************************************************
#include "Heap.h"
#include "Platform/PlatformSystemHeap.h"
#include "MimallocHeap.h"
#include "TLSFHeap.h"

namespace ob {

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
        pHeader->pHeap->Deallocate(pHeader->pRaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	ヒープの作成
    //@―---------------------------------------------------------------------------
    Heap* Heap::Create(size_t size, HeapMethodType type, const Char* pName, Heap* pParent) {
        Heap* result = nullptr;
        switch (type) {
        case ob::HeapMethodType::System:
            result = new SystemHeap();
            break;
        case ob::HeapMethodType::Mimalloc:
            result = new MimallocHeap(nullptr, 0);
            break;
        case ob::HeapMethodType::TLSF:
            LOG_ERROR_EX("Memory", "not implemented heap mode.");
            break;
        case ob::HeapMethodType::Invalid:
            LOG_ERROR_EX("Memory", "invalid heap mode.");
            break;
        default:
            LOG_ERROR_EX("Memory", "not implemented heap mode.");
            break;
        }

        return result;
    }

}// namespace ob
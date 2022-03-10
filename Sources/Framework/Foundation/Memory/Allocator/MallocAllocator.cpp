//***********************************************************
//! @file
//! @brief		システムヒープ(Windows)
//! @author		Gajumaru
//***********************************************************
#include "MallocAllocator.h"

#include <cstdlib>
#include <Windows.h>

#include <Framework/Foundation/Memory/Utility/MemoryUtility.h>

namespace ob {


    //@―---------------------------------------------------------------------------
    //! @brief	                コンストラクタ
    //@―---------------------------------------------------------------------------
    MallocAllocator::MallocAllocator(const Char* pName) {
        m_pName = pName?pName:TC("MallocAllocator");
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                デストラクタ
    //@―---------------------------------------------------------------------------
    MallocAllocator::~MallocAllocator() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                ヒープの開放
    //@―---------------------------------------------------------------------------
    void MallocAllocator::release() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                メモリの確保
    //! 
    //! @param[in] size         確保サイズ
    //! @param[in] alignment    アライメント
    //! @param[in] zeroClear    確保したバッファをゼロで初期化するか
    //@―---------------------------------------------------------------------------
    void* MallocAllocator::allocate(size_t size, u32 alignment, bool zeroClear) {
        assert(0 <= alignment);

        const size_t allocSize = sizeof(BlockHeader) + size + alignment;

        DWORD flags = 0;
        if (zeroClear)flags |= HEAP_ZERO_MEMORY;
        void* pRaw;
        if (zeroClear) {
            pRaw = calloc(1, allocSize);
        } else {
            pRaw = malloc(allocSize);
        }

        addr_t startData = (addr_t)(GetOffsetPtr(pRaw, sizeof(BlockHeader)));
        addr_t offset = 0;
        if (0 < alignment)offset = (alignment - startData % alignment) % alignment;
        void* pAlloc = GetOffsetPtr(pRaw, static_cast<s32>(sizeof(BlockHeader) + offset));


        // ヘッダ情報の登録
        auto pHeader = GetOffsetPtr<BlockHeader>(pAlloc, -static_cast<s32>(sizeof(BlockHeader)));
        if (pHeader != nullptr) {
            pHeader->pHeap = this;
            pHeader->pRaw = pRaw;
#if defined(OB_DEBUG)
            pHeader->signature = MEMORY_SIGNATURE;
            pHeader->timeStamp = 0;
#endif
        }
        return pAlloc;
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                メモリの開放
    //@―---------------------------------------------------------------------------
    void MallocAllocator::deallocate(void* pBuffer) {
        free(pBuffer);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                ヒープ名の取得
    //@―---------------------------------------------------------------------------
    const Char* MallocAllocator::name() const {
        return m_pName;
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                アロケータのヒープサイズを取得
    //@―---------------------------------------------------------------------------
    size_t MallocAllocator::heapSize() const {
        return static_cast<size_t>(-1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                アロケータの使用可能なヒープサイズを取得
    //@―---------------------------------------------------------------------------
    size_t MallocAllocator::freeHeapSize() const {
        return static_cast<size_t>(-1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                利用可能な状態かどうか
    //@―---------------------------------------------------------------------------
    bool MallocAllocator::isValid() const {
        return true;
    }

}// namespace ob
//***********************************************************
//! @file
//! @brief		システムヒープ(Windows)
//! @author		Gajumaru
//***********************************************************
#include "SystemHeap.h"

#include <cstdlib>

namespace ob
{


    //@―---------------------------------------------------------------------------
    //! @brief	                コンストラクタ
    //@―---------------------------------------------------------------------------
    SystemHeap::SystemHeap()
    {
        m_pName = TC("SystemHeap");
        m_handle = ::HeapCreate(NULL, 0, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                デストラクタ
    //@―---------------------------------------------------------------------------
    SystemHeap::~SystemHeap()
    {
        ::HeapDestroy(m_handle);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                ヒープの開放
    //@―---------------------------------------------------------------------------
    void SystemHeap::Release()
    {
    }

    //@―---------------------------------------------------------------------------
    //! @brief	                メモリの確保
    //! 
    //! @param[in] size         確保サイズ
    //! @param[in] alignment    アライメント
    //! @param[in] zeroClear    確保したバッファをゼロで初期化するか
    //@―---------------------------------------------------------------------------
    void* SystemHeap::Allocate(size_t size, u32 alignment, bool zeroClear)
    {
        assert(m_handle!=NULL);
        assert(0 <= alignment);

        const size_t allocSize = sizeof(HeapHeader) + size + alignment;

        DWORD flags = 0;
        if (zeroClear)flags |= HEAP_ZERO_MEMORY;
        auto pRaw=::HeapAlloc(m_handle, flags, allocSize);


        addr_t startData = (addr_t)(GetOffsetPtr(pRaw, sizeof(HeapHeader)));
        addr_t offset = 0;
        if (0<alignment)offset = (alignment - startData % alignment) % alignment;
        void* pAlloc = GetOffsetPtr(pRaw, static_cast<s32>(sizeof(HeapHeader) + offset));


        // ヘッダ情報の登録
        auto pHeader = GetOffsetPtr<HeapHeader>(pAlloc, -static_cast<s32>(sizeof(HeapHeader)));
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
    void SystemHeap::Deallocate(void* pBuffer)
    {
        ::HeapFree(m_handle,0,pBuffer);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                ヒープ名の取得
    //@―---------------------------------------------------------------------------
    Char* SystemHeap::GetName() const     {
        return m_pName;
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                アロケータのヒープサイズを取得
    //@―---------------------------------------------------------------------------
    size_t SystemHeap::GetHeapSize() const     {
        return static_cast<size_t>(-1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                アロケータの使用可能なヒープサイズを取得
    //@―---------------------------------------------------------------------------
    size_t SystemHeap::GetFreeHeapSize() const
    {
        return static_cast<size_t>(-1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                利用可能な状態かどうか
    //@―---------------------------------------------------------------------------
    bool SystemHeap::IsValid() const
    {
        return true;
    }

}// namespace ob
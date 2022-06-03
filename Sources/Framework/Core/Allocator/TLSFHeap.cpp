//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "TLSFHeap.h"
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

    static s32 s_zeroSize = 0;


    const size_t TLSFHeap::s_blockManageSize = sizeof(TLSFHeap::BlockTag) + sizeof(u32);// タグ+アライメント管理
    const size_t TLSFHeap::s_blockMinimumSize = TLSFHeap::s_blockManageSize + sizeof(HeapHeader);

#if defined(OS_64BIT)
    const s32    TLSFHeap::s_secendLevelDivCount = 3;
#elif defined (OS_32BIT)
    const s32    TLSFHeap::s_secendLevelDivCount = 2;
#endif
    const s32    TLSFHeap::s_secendLevelDivNum = (1 << TLSFHeap::s_secendLevelDivCount);

    const s32    TLSFHeap::s_linerManagementIndex = 6;
    const s32    TLSFHeap::s_linerManagementSize = (1 << TLSFHeap::s_linerManagementIndex);
    const s32    TLSFHeap::s_linerManagementDivSize = (TLSFHeap::s_linerManagementSize / TLSFHeap::s_secendLevelDivNum);


    //@―---------------------------------------------------------------------------

    TLSFHeap::TLSFHeap(size_t size)
        : m_pName(TC(""))
        , m_allSize(size)
        , m_allAlocSize(0)
        , m_maxDivideNum(0)
        , m_pBaseMem(nullptr)
        , m_freeList(nullptr)
        , m_activeList(nullptr)

        , m_freeListBitFLI(0)
        , freeListBitSLI(nullptr)
        , freeListBit(false)
        , activeListBit(false)
        , BNS(0)
        , BBS(0) {

    }

    TLSFHeap::~TLSFHeap() {

    }

    void TLSFHeap::Clear() {

    }

    void* TLSFHeap::allocate(size_t size, u32 alignment, bool zeroClear) {
        return nullptr;
    }

    void TLSFHeap::deallocate(void* pBuffer) {
    }

    void TLSFHeap::release() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief	                ヒープ名を取得
    //@―---------------------------------------------------------------------------
    Char* TLSFHeap::name() const {
        return m_pName;
    }

    size_t TLSFHeap::heapSize() const {
        return size_t();
    }

    size_t TLSFHeap::freeHeapSize() const {
        return size_t();
    }

    bool TLSFHeap::isValid() const {
        return false;
    }





    TLSFHeap::BlockTag::BlockTag()
        :m_flag(false)
        , m_size(0)
        , m_pData(nullptr)
        , m_pAllSize(nullptr) {
        m_pPrev = m_pNext = this;
    }


    TLSFHeap::BlockTag::BlockTag(byte* pData, s32 size)
        :m_flag(false)
        , m_pData(pData) {
        OB_CHECK_ASSERT_EXPR(size < 0);
        m_pPrev = m_pNext = this;
        m_size = size;
        if (m_pData != nullptr) {
            m_pAllSize = reinterpret_cast<s32*>(m_pData + size);
            (*m_pAllSize) = sizeof(BlockTag) + size + sizeof(s32);
        } else {
            m_pAllSize = &s_zeroSize;
        }
    }


    TLSFHeap::BlockTag::~BlockTag() {

    }

    // 運用データサイズを取得
    s32 TLSFHeap::BlockTag::GetDataSize() {
        return m_size;
    }

    // 自身が占有するすべてのデータサイズを取得
    s32 TLSFHeap::BlockTag::GetAllSize() {
        return *m_pAllSize;
    }

    // 双方向リストへ入れる
    TLSFHeap::BlockTag* TLSFHeap::BlockTag::Register(BlockTag* nbp) {
        //呼び出し元は常にダミーリストの先頭ブロック
        nbp->m_pNext = m_pNext;
        nbp->m_pPrev = this;
        nbp->m_pPrev->m_pNext = nbp->m_pNext->m_pPrev = nbp;
        return this;
    }

    // 双方向リストから取り除く
    TLSFHeap::BlockTag* TLSFHeap::BlockTag::Remove() {
        m_pPrev->m_pNext = m_pNext;
        m_pNext->m_pPrev = m_pPrev;
        m_pPrev = m_pNext = this;
        return this;
    }

}// namespace ob
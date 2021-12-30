//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "RefObject.h"
#include <Runtime/Foundation/Atomic/Atomic.h>


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    RefObject::RefObject()
        :m_referenceCount(0) {
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    RefObject::~RefObject() {
        auto count = AtomicOperator::Load(&m_referenceCount);
        OB_REQUIRE(count <= 0);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  参照カウントの取得
    //@―---------------------------------------------------------------------------
    s32 RefObject::GetReferenceCount()const noexcept {
        return AtomicOperator::Load(&m_referenceCount);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照の追加
    //@―---------------------------------------------------------------------------
    s32 RefObject::Retain() {
        return AtomicOperator::Increment(&m_referenceCount);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  参照の解放
    //@―---------------------------------------------------------------------------
    s32 RefObject::Release() {
        s32 count = AtomicOperator::Decrement(&m_referenceCount);
        if (count <= 0) {
            delete this;
        }
        return count;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  参照カウントの取得
    //! 
    //! @param pObj 捜査対象の IRefObject
    //@―---------------------------------------------------------------------------
    s32 RefObjectHelper::GetReferenceCount(RefObject* pObj) {
        if (pObj == nullptr)return 0;
        return pObj->GetReferenceCount();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照の追加
    //! 
    //! @param pObj 捜査対象の IRefObject
    //@―---------------------------------------------------------------------------
    s32 RefObjectHelper::Retain(RefObject* pObj) {
        if (pObj == nullptr)return 0;
        return pObj->Retain();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照の解放
    //! 
    //! @param pObj 捜査対象の IRefObject
    //@―---------------------------------------------------------------------------
    s32 RefObjectHelper::Release(RefObject* pObj) {
        if (pObj == nullptr)return 0;
        return pObj->Release();
    }

}// namespace ob
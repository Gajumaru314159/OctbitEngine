//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#include "GraphicObject.h"
#include <Framework/Graphic/System.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::GraphicObject(StringView name)
        : m_pStack(nullptr)
        , m_referenceCount(1){

        OB_CHECK_ASSERT_EX(name.size()<get_size(m_name), "グラフィック・オブジェクトに設定する名前は{0}文字以下にしてください。[ name={1} ]", get_size(m_name), name.data());
        //System::Instance().registerObject(*this);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::~GraphicObject() {
        OB_CHECK_ASSERT_EX(m_referenceCount == 0, "グラフィック・オブジェクトの参照カウンタが不正です。[ name = {0}]", getName());
    }


    //@―---------------------------------------------------------------------------
    //! @brief  名前を取得
    //@―---------------------------------------------------------------------------
    const Char* GraphicObject::getName()const {
#ifdef OB_DEBUG
        return m_name;
#else
        return TC("Unknown");
#endif
    }

    
    //@―---------------------------------------------------------------------------
    //! @brief  参照を追加
    //@―---------------------------------------------------------------------------
    void GraphicObject::addReference() {
        m_referenceCount.fetch_add(1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照を解放
    //@―---------------------------------------------------------------------------
    s32 GraphicObject::releaseReference() {
        m_referenceCount.fetch_sub(1);
        if (1 < m_referenceCount) {
            return m_referenceCount;
        }
        OB_CHECK_ASSERT_EX(m_referenceCount == 0, "参照カウントエラー");
        // デバイスの削除スタックに追加
        System::Instance().requestRelease(*this);
        return m_referenceCount;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照カウントを取得
    //@―---------------------------------------------------------------------------
    s32 GraphicObject::getReferenceCount()const {
        return m_referenceCount;
    }
    



}// namespace ob::graphic
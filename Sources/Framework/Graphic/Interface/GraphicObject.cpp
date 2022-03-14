//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#include "GraphicObject.h"

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::GraphicObject(StringView name)
        : m_pStack(nullptr)
        , m_name(name){
        // TODO できればFixedStringにしたい
        //m_refCount.store(0, memory_order::memory_order_release);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::~GraphicObject() {
        auto pName = TC("Unknown");
#ifdef OB_DEBUG
        if (!m_name.empty())pName = m_name.c_str();
#endif
        OB_REQUIRE_EX(m_refCount == 0, "グラフィック・オブジェクトの参照カウンタが不正です。Object={0}", pName);
    }

    /*
    //@―---------------------------------------------------------------------------
    //! @brief  参照を追加
    //@―---------------------------------------------------------------------------
    void GraphicObject::addRef() {
        m_refCount.fetch_add(1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照を解放
    //@―---------------------------------------------------------------------------
    void GraphicObject::releaseRef() {
        m_refCount.fetch_sub(1);
        if (1 < m_refCount) {
            return;
        }
        OB_REQUIRE_EX(m_refCount == 0, "参照カウントエラー");
        // デバイスの削除スタックに追加
        // m_device.entryDeleteStack(*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  参照カウントを取得
    //@―---------------------------------------------------------------------------
    s32 GraphicObject::getRefCount()const {
        return m_refCount;
    }
    */

    //@―---------------------------------------------------------------------------
    //! @brief  名前を取得
    //@―---------------------------------------------------------------------------
    const StringView GraphicObject::getName()const {
#ifdef OB_DEBUG
        return m_name;
#else
        return TC("The name of the graphic object can only be used during debugging.");
#endif
    }



}// namespace ob::graphic
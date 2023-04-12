//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/GraphicObjectManager.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::GraphicObject()
    {
        if (auto manager = GraphicObjectManager::Get()) {
            manager->registerObject(*this);
            m_managed = true;
        } else {
            m_managed = false;
        }

    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::~GraphicObject() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      終了処理
    //@―---------------------------------------------------------------------------
    void GraphicObject::finalize() {
        if (m_managed) {
            if (auto manager = GraphicObjectManager::Get()) {
                manager->requestRelease(*this);
            }
        }
    }

}
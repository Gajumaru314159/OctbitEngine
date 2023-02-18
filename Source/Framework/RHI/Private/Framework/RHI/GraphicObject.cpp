//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/RHIModule.h>
#include <Framework/Engine/Engine.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::GraphicObject()
    {
        static RHIModule* pModule = nullptr;
        if (pModule == nullptr) {
            pModule = GEngine->get<RHIModule>();
        }
        OB_ASSERT(pModule != nullptr, "Graphicモジュールがありません");

        pModule->getObjectManager().registerObject(*this);
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

        static RHIModule* pModule = nullptr;
        if (pModule == nullptr) {
            pModule = GEngine->get<RHIModule>();
        }
        OB_ASSERT(pModule != nullptr, "Graphicモジュールがありません");

        pModule->getObjectManager().requestRelease(*this);

    }

}// namespace ob::rhi
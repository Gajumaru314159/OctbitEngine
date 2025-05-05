//***********************************************************
//! @file
//! @brief		RHI
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/GraphicObjectManager.h>

namespace ob::rhi {

    //! @brief  コンストラクタ
    RHI::RHI(GraphicObjectManager& manager,RHIConfig* config) 
        : m_objectManager(manager)
        , m_config(config? *config:RHIConfig())
    {
    }


    //! @brief  デストラクタ
    RHI::~RHI() {
    }


    //! @brief  更新
    void RHI::update() {
        m_objectManager.update();
    }


    //! @brief  終了処理
    void RHI::finalize() {
        m_objectManager.finalize();
    }
}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/GraphicObjectManager.h>

namespace ob::rhi {

    //! @brief  コンストラクタ
    Device::Device(GraphicObjectManager& manager,RHIConfig* config) 
        : m_objectManager(manager)
        , m_config(config? *config:RHIConfig())
    {
    }


    //! @brief  デストラクタ
    Device::~Device() {
    }


    //! @brief  更新
    void Device::update() {
        m_objectManager.update();
    }


    //! @brief  終了処理
    void Device::finalize() {
        m_objectManager.finalize();
    }
}
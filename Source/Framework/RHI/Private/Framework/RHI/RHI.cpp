//***********************************************************
//! @file
//! @brief		RHI
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/GraphicObjectManager.h>

namespace ob::rhi {

    static RHI* s_instance = nullptr;

    RHI* RHI::Get() {
        return s_instance;
    }

    RHI::RHI(GraphicObjectManager& manager) 
        :m_objectManager(manager)
    {
        OB_ASSERT(s_instance == nullptr, "{}は既に生成されています。", TypeId::Get<decltype(this)>().name());
        s_instance = this;
    }
    RHI::~RHI() {
        s_instance = nullptr;
    }
    void RHI::update() {
        m_objectManager.update();
    }
    void RHI::finalize() {
        for (s32 i = 0; i < m_objectManager.getFrameBufferCount(); ++i) {
            m_objectManager.update();
        }
    }
}
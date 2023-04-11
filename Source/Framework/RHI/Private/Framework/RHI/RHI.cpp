//***********************************************************
//! @file
//! @brief		RHI
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    static RHI* s_instance = nullptr;

    RHI* RHI::Get() {
        return s_instance;
    }

    RHI::RHI() {
        OB_ASSERT(s_instance == nullptr, "{}は既に生成されています。", TypeId::Get<decltype(this)>().name());
        s_instance = this;
    }
    RHI::~RHI() {
        s_instance = nullptr;
    }

}// namespace ob::rhi
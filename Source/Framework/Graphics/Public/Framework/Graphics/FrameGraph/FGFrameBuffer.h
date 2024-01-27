//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/FrameBufferDesc.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      FrameGraph中間フレームバッファ
    //@―---------------------------------------------------------------------------
    class FGFrameBuffer {
    public:
        using Desc = rhi::FrameBufferDesc;

        void create(const Desc& desc, void* allocator);
        void destroy(const Desc& desc, void* allocator);
        static std::string toStringt(const Desc& desc);

        Ref<rhi::FrameBuffer> instance;
    };

}
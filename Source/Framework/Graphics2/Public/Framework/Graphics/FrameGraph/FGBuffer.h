//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/BufferDesc.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      FrameGraph中間テクスチャ
    //@―---------------------------------------------------------------------------
    class FGBuffer{
    public:
        using Desc = rhi::BufferDesc;

        void create(const Desc& desc, void* allocator);
        void destroy(const Desc& desc, void* allocator);
        static std::string toStringt(const Desc& desc);

        Ref<rhi::Buffer> instance;
    };

}
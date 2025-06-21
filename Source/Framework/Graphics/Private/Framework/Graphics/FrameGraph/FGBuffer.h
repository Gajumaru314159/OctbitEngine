//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/BufferDesc.h>

namespace ob::graphics {

    //! @brief      FrameGraph中間テクスチャ
    class FGBuffer{
    public:
        using Desc = rhi::BufferDesc;

        void create(const Desc& desc, void* allocator);
        void destroy(const Desc& desc, void* allocator);
        static std::string toString(const Desc& desc);

        void preRead(const Desc& desc, uint32_t flags, void* ctx);
        void preWrite(const Desc& desc, uint32_t flags, void* ctx);

        Ref<rhi::Buffer> instance;
    };

}
//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/RenderTextureDesc.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      FrameGraph中間テクスチャ
    //@―---------------------------------------------------------------------------
    class FGTexture {
    public:
        using Desc = rhi::RenderTextureDesc;

        void create(const Desc& desc, void* allocator);
        void destroy(const Desc& desc, void* allocator);
        static std::string toStringt(const Desc& desc);

        Ref<rhi::RenderTexture> instance;
    };

}
//***********************************************************
//! @file
//! @brief		レンダーテクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief          生成
    //@―---------------------------------------------------------------------------
    Ref<RenderTexture> RenderTexture::Create(const RenderTextureDesc& desc) {
        if (auto rhi= RHI::Get()) {
            return rhi->createRenderTexture(desc);;
        }
        return nullptr;
    }

}
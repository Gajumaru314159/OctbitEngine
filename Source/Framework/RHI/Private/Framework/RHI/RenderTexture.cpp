//***********************************************************
//! @file
//! @brief		描画テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief          生成
    //@―---------------------------------------------------------------------------
    Ref<RenderTexture> RenderTexture::Create(const RenderTextureDesc& desc) {
        if (auto device = Device::Get()) {
            return device->createRenderTexture(desc);;
        }
        return nullptr;
    }

}// namespace ob
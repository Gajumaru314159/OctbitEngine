//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi
{

    //@―---------------------------------------------------------------------------
    //! @brief          生成
    //@―---------------------------------------------------------------------------
    Ref<RenderTexture> RenderTexture::Create(const RenderTextureDesc& desc) {
        return Device::Get()->createRenderTexture(desc);;
    }

}// namespace ob
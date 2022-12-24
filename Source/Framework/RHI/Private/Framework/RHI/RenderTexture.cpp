//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/IRenderTexture.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi
{
    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(RenderTexture);

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc     定義
    //! @param name     オブジェクト名
    //@―---------------------------------------------------------------------------
    RenderTexture::RenderTexture(const RenderTextureDesc& desc) {
        m_pImpl = Device::Get()->createRenderTexture(desc);
        Texture::m_pImpl = m_pImpl;
        OB_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "RenderTextureの生成に失敗");
            release();
        }
    }

}// namespace ob
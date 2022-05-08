//***********************************************************
//! @file
//! @brief		リソース・バリア
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/ResourceBarrier.h>
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/RenderTarget.h>
#include <Framework/Graphic/Interface/ISwapchain.h>
#include <Framework/Graphic/Interface/ITexture.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void ResourceBarrier::add(const Texture& texture, ResourceState prev, ResourceState next, s32 arrayIndex, s32 mipIndex) {
        TextureBarrier barrier;
        barrier.pTexture = &texture;
        barrier.prev = prev;
        barrier.next = next;
        barrier.arrayIndex = arrayIndex;
        barrier.mipIndex = mipIndex;
        m_textureBarriers.push_back(barrier);
    }

}// namespace ob::graphic
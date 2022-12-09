//***********************************************************
//! @file
//! @brief		リソース・バリア
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/ResourceBarrier.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/RenderTarget.h>
#include <Framework/RHI/ITexture.h>

namespace ob::rhi {

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

}// namespace ob::rhi
//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Runtime/GraphicD3D12/Texture/D3D12Texture.h>

namespace ob {
    namespace graphic {



        D3D12Texture::D3D12Texture() {

        }


        D3D12Texture::D3D12Texture(ID3D12Device* pDevice, StringView path) {
            OB_REQUIRE(pDevice != null);
        }

        IntVec2 D3D12Texture::size()const noexcept {

        }

        //TextureFormat getFormat()const;

        bool D3D12Texture::isMipped()const noexcept {

        }

        bool D3D12Texture::isSRGB()const noexcept {

        }

        bool D3D12Texture::hasDepth()const noexcept {

        }
    }
}
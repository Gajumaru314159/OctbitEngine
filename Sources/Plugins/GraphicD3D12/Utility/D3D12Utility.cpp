//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "D3D12Utility.h"

namespace ob {
    namespace graphic {
        
        //@―---------------------------------------------------------------------------
        //! @brief  TextureFormat を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        DXGI_FORMAT D3D12Utility::convertTextureFormat(TextureFormat format) {
            switch (format) {
            case ob::graphic::TextureFormat::RGBA32:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case ob::graphic::TextureFormat::RGBA16:
                return DXGI_FORMAT_R16G16B16A16_FLOAT;
            case ob::graphic::TextureFormat::RGBA8:
                return DXGI_FORMAT_R8G8B8A8_UNORM;

            case ob::graphic::TextureFormat::RGB32:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case ob::graphic::TextureFormat::RGB8:
                return DXGI_FORMAT_R8G8_UNORM;

            case ob::graphic::TextureFormat::RG32:
                return DXGI_FORMAT_R32G32_FLOAT;
            case ob::graphic::TextureFormat::RG16:
                return DXGI_FORMAT_R16G16_FLOAT;
            case ob::graphic::TextureFormat::RG8:
                return DXGI_FORMAT_R8G8_UNORM;

            case ob::graphic::TextureFormat::R32:
                return DXGI_FORMAT_R32_FLOAT;
            case ob::graphic::TextureFormat::R16:
                return DXGI_FORMAT_R16_FLOAT;
            case ob::graphic::TextureFormat::R8:
                return DXGI_FORMAT_R8_UNORM;

            case ob::graphic::TextureFormat::BC1:
                return DXGI_FORMAT_BC1_UNORM;
            case ob::graphic::TextureFormat::BC2:
                return DXGI_FORMAT_BC2_UNORM;
            case ob::graphic::TextureFormat::BC3:
                return DXGI_FORMAT_BC3_UNORM;
            case ob::graphic::TextureFormat::BC4:
                return DXGI_FORMAT_BC4_UNORM;
            case ob::graphic::TextureFormat::BC5:
                return DXGI_FORMAT_BC5_UNORM;
            case ob::graphic::TextureFormat::BC6H:
                return DXGI_FORMAT_BC6H_UF16;
            case ob::graphic::TextureFormat::BC7:
                return DXGI_FORMAT_BC7_UNORM;

            case ob::graphic::TextureFormat::BC1_SRGB:
                return DXGI_FORMAT_BC1_UNORM_SRGB;
            case ob::graphic::TextureFormat::BC2_SRGB:
                return DXGI_FORMAT_BC2_UNORM_SRGB;
            case ob::graphic::TextureFormat::BC3_SRGB:
                return DXGI_FORMAT_BC3_UNORM_SRGB;
            case ob::graphic::TextureFormat::BC7_SRGB:
                return DXGI_FORMAT_BC7_UNORM_SRGB;
            default:
                break;
            }
        }

    }// namespace graphic
}// namespace ob
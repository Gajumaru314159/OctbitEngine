//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Utility.h"
#include <Framework/Foundation/String/StringEncoder.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  TextureFormat を DXGI_FORMAT に変換
    //@―---------------------------------------------------------------------------
    DXGI_FORMAT Utility::convertTextureFormat(TextureFormat format) {
        switch (format) {
        case TextureFormat::RGBA32:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case TextureFormat::RGBA16:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case TextureFormat::RGBA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM;

        case TextureFormat::RGB32:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case TextureFormat::RGB8:
            return DXGI_FORMAT_B8G8R8X8_UNORM;

        case TextureFormat::RG32:
            return DXGI_FORMAT_R32G32_FLOAT;
        case TextureFormat::RG16:
            return DXGI_FORMAT_R16G16_FLOAT;
        case TextureFormat::RG8:
            return DXGI_FORMAT_R8G8_UNORM;

        case TextureFormat::R32:
            return DXGI_FORMAT_R32_FLOAT;
        case TextureFormat::R16:
            return DXGI_FORMAT_R16_FLOAT;
        case TextureFormat::R8:
            return DXGI_FORMAT_R8_UNORM;

        case TextureFormat::R10G10B10A2:
            return DXGI_FORMAT_R10G10B10A2_UNORM;

        case TextureFormat::D32S8:
            return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case TextureFormat::D32:
            return DXGI_FORMAT_D32_FLOAT;
        case TextureFormat::D24S8:
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormat::D16:
            return DXGI_FORMAT_D16_UNORM;

        case TextureFormat::BC1:
            return DXGI_FORMAT_BC1_UNORM;
        case TextureFormat::BC2:
            return DXGI_FORMAT_BC2_UNORM;
        case TextureFormat::BC3:
            return DXGI_FORMAT_BC3_UNORM;
        case TextureFormat::BC4:
            return DXGI_FORMAT_BC4_UNORM;
        case TextureFormat::BC5:
            return DXGI_FORMAT_BC5_UNORM;
        case TextureFormat::BC6H:
            return DXGI_FORMAT_BC6H_UF16;
        case TextureFormat::BC7:
            return DXGI_FORMAT_BC7_UNORM;

        case TextureFormat::BC1_SRGB:
            return DXGI_FORMAT_BC1_UNORM_SRGB;
        case TextureFormat::BC2_SRGB:
            return DXGI_FORMAT_BC2_UNORM_SRGB;
        case TextureFormat::BC3_SRGB:
            return DXGI_FORMAT_BC3_UNORM_SRGB;
        case TextureFormat::BC7_SRGB:
            return DXGI_FORMAT_BC7_UNORM_SRGB;
        }
        return DXGI_FORMAT_UNKNOWN;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  エラーメッセージを取得
    //@―---------------------------------------------------------------------------
    String Utility::getErrorMessage(DWORD errorCode) {
        WCHAR buffer[256];
        auto result = ::FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorCode,
            buffer,
            sizeof(buffer),
            nullptr
        );

        String error=fmt::format(TC("[{0:#X}]"),errorCode);
        if (0 < result) {
            String message;
            StringEncoder::Encode(buffer, message);
            error += message;
        }
        return move(error);
    }


    D3D12_SRV_DIMENSION Utility::getSrvDimention(const D3D12_RESOURCE_DESC& desc) {
        switch (desc.Dimension) {
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
            if (desc.DepthOrArraySize == 0) {
                return D3D12_SRV_DIMENSION_TEXTURE1D;
            } else {
                return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
            }
            break;

        case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
            if (desc.DepthOrArraySize == 0) {
                return D3D12_SRV_DIMENSION_TEXTURE2D;
            } else {
                return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            }
            break;

        case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
            return D3D12_SRV_DIMENSION_TEXTURE3D;

        case D3D12_RESOURCE_DIMENSION_BUFFER:
            return D3D12_SRV_DIMENSION_BUFFER;
        }
        return D3D12_SRV_DIMENSION_UNKNOWN;

        // D3D12_SRV_DIMENSION_TEXTURE2DMS
        // D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY
        // D3D12_SRV_DIMENSION_TEXTURECUBE
        // D3D12_SRV_DIMENSION_TEXTURECUBEARRAY
        // D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE
    }

}// namespace ob::graphic::dx12
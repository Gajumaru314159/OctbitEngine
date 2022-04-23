//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#include "TypeConverter.h"
#include "MagicEnum/magic_enum.hpp"

namespace ob::graphic::dx12
{
    //@―---------------------------------------------------------------------------
    //! @brief  RootSignatureFlags を D3D12_ROOT_SIGNATURE_FLAGS に変換
    //@―---------------------------------------------------------------------------
    D3D12_ROOT_SIGNATURE_FLAGS TypeConverter::convert(RootSignatureFlags value) {
        D3D12_ROOT_SIGNATURE_FLAGS result = D3D12_ROOT_SIGNATURE_FLAG_NONE;
        if (value[RootSignatureFlag::AllowInputAssemblerInputLayout])   result |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        if (value[RootSignatureFlag::DenyVertexShaderAccess])   result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
        if (value[RootSignatureFlag::DenyHullShaderAccess])     result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        if (value[RootSignatureFlag::DenyDomainShaderAccess])   result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        if (value[RootSignatureFlag::DenyGeometryShaderAccess]) result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
        if (value[RootSignatureFlag::DenyPixelShaderAccess])    result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
        if (value[RootSignatureFlag::AllowStreamOutput])        result |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
        if (value[RootSignatureFlag::DenyAmplificationShaderAccess])result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS;
        if (value[RootSignatureFlag::DenyMeshShaderAccess])     result |= D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;
        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_ROOT_PARAMETER_TYPE TypeConverter::convert(RootParameterType value) {
        switch (value) {
        case RootParameterType::CBV:
            return D3D12_ROOT_PARAMETER_TYPE_CBV;
        case RootParameterType::SRV:
            return D3D12_ROOT_PARAMETER_TYPE_SRV;
        case RootParameterType::UAV:
            return D3D12_ROOT_PARAMETER_TYPE_UAV;
        case RootParameterType::DescriptorTable:
            return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        case RootParameterType::RootConstants:
            return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        }
        LOG_WARNING_EX("Graphic", "不正なRootParameterType[value={}]", enum_cast(value));
        return D3D12_ROOT_PARAMETER_TYPE_CBV;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_DESCRIPTOR_RANGE_TYPE TypeConverter::convert(DescriptorRangeType value) {
        switch (value) {
        case DescriptorRangeType::CBV:
            return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        case DescriptorRangeType::SRV:
            return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        case DescriptorRangeType::UAV:
            return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        case DescriptorRangeType::Sampler:
            return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
        }
        LOG_WARNING_EX("Graphic", "不正なDescriptorRangeType[value={}]", enum_cast(value));
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ShaderStages を D3D12_SHADER_VISIBILITY に変換
    //@―---------------------------------------------------------------------------
    D3D12_SHADER_VISIBILITY TypeConverter::convert(ShaderStage value) {
        switch (value) {
        case ShaderStage::Vertex:
            return D3D12_SHADER_VISIBILITY_VERTEX;
        case ShaderStage::Hull:
            return D3D12_SHADER_VISIBILITY_HULL;
        case ShaderStage::Domain:
            return D3D12_SHADER_VISIBILITY_DOMAIN;
        case ShaderStage::Geometry:
            return D3D12_SHADER_VISIBILITY_GEOMETRY;
        case ShaderStage::Pixel:
            return D3D12_SHADER_VISIBILITY_PIXEL;
        case ShaderStage::Amplification:
            return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
        case ShaderStage::Mesh:
            return D3D12_SHADER_VISIBILITY_MESH;
        case ShaderStage::All:
            return D3D12_SHADER_VISIBILITY_ALL;
        }

        LOG_WARNING_EX("Graphic", "不正なShaderStage[value={}]", enum_cast(value));
        return D3D12_SHADER_VISIBILITY_ALL;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  TextureFillter を D3D12_FILTER_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_FILTER_TYPE TypeConverter::convert(TextureFillter value){
        switch (value) {
        case TextureFillter::Point:
            return D3D12_FILTER_TYPE_POINT;
        case TextureFillter::Linear:
            return D3D12_FILTER_TYPE_LINEAR;
        }

        LOG_WARNING_EX("Graphic", "不正なTextureFillter[value={}]", enum_cast(value));
        return D3D12_FILTER_TYPE_LINEAR;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  MipFillter を D3D12_DESCRIPTOR_RANGE_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_FILTER_TYPE TypeConverter::convert(MipFillter value){
        switch (value) {
        case MipFillter::Point:
            return D3D12_FILTER_TYPE_POINT;
        case MipFillter::Linear:
            return D3D12_FILTER_TYPE_LINEAR;
        }
        LOG_WARNING_EX("Graphic", "不正なMipFillter[value={}]", enum_cast(value));
        return D3D12_FILTER_TYPE_LINEAR;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  D3D12_FILTER に変換
    //@―---------------------------------------------------------------------------
    D3D12_FILTER TypeConverter::convert(TextureFillter up, TextureFillter down, MipFillter mip, bool anisotropic) {
        if (anisotropic)return D3D12_FILTER_ANISOTROPIC;
        return D3D12_ENCODE_BASIC_FILTER(
            convert(down),  //!< 縮小時
            convert(up),    //!< 拡大時
            convert(mip),   //!< ミップ
            D3D12_FILTER_REDUCTION_TYPE_STANDARD    // 1次元カラーのみ変更可能
        );
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Anisotropy を UINT に変換
    //@―---------------------------------------------------------------------------
    UINT TypeConverter::convert(Anisotropy value){
        switch (value)
        {
        case Anisotropy::None:
            return 0;
        case Anisotropy::Level1:
            return 1;
        case Anisotropy::Level2:
            return 2;
        case Anisotropy::Level4:
            return 4;
        case Anisotropy::Level8:
            return 8;
        case Anisotropy::Level16:
            return 16;
        }
        LOG_WARNING_EX("Graphic", "不正なAnisotropy[value={}]", enum_cast(value));
        return 0;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  TextureAddress を D3D12_TEXTURE_ADDRESS_MODE に変換
    //@―---------------------------------------------------------------------------
    D3D12_TEXTURE_ADDRESS_MODE TypeConverter::convert(TextureAddress value){
        switch (value)
        {
        case ob::graphic::TextureAddress::Repeat:
            return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        case ob::graphic::TextureAddress::Clamp:
            return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        case ob::graphic::TextureAddress::Mirror:
            return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
        }
        LOG_WARNING_EX("Graphic", "不正なAnisotropy[value={}]", enum_cast(value));
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  TextureFormat を DXGI_FORMAT に変換
    //@―---------------------------------------------------------------------------
    DXGI_FORMAT TypeConverter::convert(TextureFormat value) {
        switch (value) {
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

        LOG_WARNING_EX("Graphic", "不正なAnisotropy[value={}]", enum_cast(value));
        return DXGI_FORMAT_UNKNOWN;
    }

}// namespace ob
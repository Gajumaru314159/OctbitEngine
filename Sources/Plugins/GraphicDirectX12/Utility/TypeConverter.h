//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/RootSignatureDesc.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  DirectXの内部型変換
    //@―---------------------------------------------------------------------------
    class TypeConverter {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  RootSignatureFlags を D3D12_ROOT_SIGNATURE_FLAGS に変換
        //@―---------------------------------------------------------------------------
        static D3D12_ROOT_SIGNATURE_FLAGS convert(RootSignatureFlags value);


        //@―---------------------------------------------------------------------------
        //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_ROOT_PARAMETER_TYPE convert(RootParameterType value);


        //@―---------------------------------------------------------------------------
        //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_DESCRIPTOR_RANGE_TYPE convert(DescriptorRangeType value);


        //@―---------------------------------------------------------------------------
        //! @brief  ShaderStage を D3D12_SHADER_VISIBILITY に変換
        //@―---------------------------------------------------------------------------
        static D3D12_SHADER_VISIBILITY convert(ShaderStage value);


        //@―---------------------------------------------------------------------------
        //! @brief  TextureFillter を D3D12_FILTER_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILTER_TYPE convert(TextureFillter value);


        //@―---------------------------------------------------------------------------
        //! @brief  MipFillter を D3D12_DESCRIPTOR_RANGE_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILTER_TYPE convert(MipFillter value);


        //@―---------------------------------------------------------------------------
        //! @brief  D3D12_FILTER に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILTER convert(TextureFillter up, TextureFillter down, MipFillter mip, bool anisotropic);


        //@―---------------------------------------------------------------------------
        //! @brief  Anisotropy を UINT に変換
        //@―---------------------------------------------------------------------------
        static UINT convert(Anisotropy value);


        //@―---------------------------------------------------------------------------
        //! @brief  TextureAddress を D3D12_TEXTURE_ADDRESS_MODE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_TEXTURE_ADDRESS_MODE convert(TextureAddress value);


        //@―---------------------------------------------------------------------------
        //! @brief  TextureFormat を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        static DXGI_FORMAT convert(TextureFormat value);

    private:



    };
}// namespcae ob
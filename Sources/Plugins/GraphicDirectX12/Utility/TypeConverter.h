//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/RootSignatureDesc.h>
#include <Framework/Graphic/Types/PipelineStateDesc.h>

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
        //! @brief  FillMode を D3D12_FILL_MODE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILL_MODE convert(FillMode value);
        //@―---------------------------------------------------------------------------
        //! @brief  CullMode を D3D12_CULL_MODE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_CULL_MODE convert(CullMode value);
        //@―---------------------------------------------------------------------------
        //! @brief  TextureFormat を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        static DXGI_FORMAT convert(TextureFormat value);
        //@―---------------------------------------------------------------------------
        //! @brief  BlendFactor を D3D12_BLEND に変換
        //@―---------------------------------------------------------------------------
        static D3D12_BLEND convert(BlendFactor value);
        //@―---------------------------------------------------------------------------
        //! @brief  BlendOp を D3D12_BLEND_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_BLEND_OP convert(BlendOp value);
        //@―---------------------------------------------------------------------------
        //! @brief  BlendFactor を UINT8 に変換
        //@―---------------------------------------------------------------------------
        static UINT8 convert(ColorMask value);
        //@―---------------------------------------------------------------------------
        //! @brief  LogicOp を D3D12_LOGIC_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_LOGIC_OP convert(LogicOp value);
        //@―---------------------------------------------------------------------------
        //! @brief  Topology を D3D12_LOGIC_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_PRIMITIVE_TOPOLOGY_TYPE convert(Topology value);
        //@―---------------------------------------------------------------------------
        //! @brief  Type を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        static DXGI_FORMAT convert(Type value,s32 dimension);
        //@―---------------------------------------------------------------------------
        //! @brief  Semantic を LPCSTR に変換
        //@―---------------------------------------------------------------------------
        static LPCSTR convert(Semantic value);
        //@―---------------------------------------------------------------------------
        //! @brief  StencilOp を D3D12_STENCIL_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_STENCIL_OP convert(StencilOp value);
        //@―---------------------------------------------------------------------------
        //! @brief  ComparisonFunc を D3D12_COMPARISON_FUNC に変換
        //@―---------------------------------------------------------------------------
        static D3D12_COMPARISON_FUNC convert(ComparisonFunc value);

    private:



    };
}// namespcae ob
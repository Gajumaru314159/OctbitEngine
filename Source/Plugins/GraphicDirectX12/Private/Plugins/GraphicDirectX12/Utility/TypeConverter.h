//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/RootSignatureDesc.h>
#include <Framework/Graphic/Types/PipelineStateDesc.h>
#include <Framework/Graphic/Types/DescriptorDesc.h>
#include <Framework/Graphic/Types/CommandListDesc.h>
#include <Framework/Graphic/Types/BufferDesc.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  DirectXの内部型変換
    //@―---------------------------------------------------------------------------
    class TypeConverter {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  RootSignatureFlags を D3D12_ROOT_SIGNATURE_FLAGS に変換
        //@―---------------------------------------------------------------------------
        static D3D12_ROOT_SIGNATURE_FLAGS Convert(RootSignatureFlags value);
        //@―---------------------------------------------------------------------------
        //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_ROOT_PARAMETER_TYPE Convert(RootParameterType value);
        //@―---------------------------------------------------------------------------
        //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_DESCRIPTOR_RANGE_TYPE Convert(DescriptorRangeType value);
        //@―---------------------------------------------------------------------------
        //! @brief  ShaderStage を D3D12_SHADER_VISIBILITY に変換
        //@―---------------------------------------------------------------------------
        static D3D12_SHADER_VISIBILITY Convert(ShaderStage value);
        //@―---------------------------------------------------------------------------
        //! @brief  TextureFillter を D3D12_FILTER_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILTER_TYPE Convert(TextureFillter value);
        //@―---------------------------------------------------------------------------
        //! @brief  MipFillter を D3D12_DESCRIPTOR_RANGE_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILTER_TYPE Convert(MipFillter value);
        //@―---------------------------------------------------------------------------
        //! @brief  D3D12_FILTER に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILTER Convert(TextureFillter up, TextureFillter down, MipFillter mip, bool anisotropic);
        //@―---------------------------------------------------------------------------
        //! @brief  Anisotropy を UINT に変換
        //@―---------------------------------------------------------------------------
        static UINT Convert(Anisotropy value);
        //@―---------------------------------------------------------------------------
        //! @brief  TextureAddress を D3D12_TEXTURE_ADDRESS_MODE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_TEXTURE_ADDRESS_MODE Convert(TextureAddress value);
        //@―---------------------------------------------------------------------------
        //! @brief  FillMode を D3D12_FILL_MODE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_FILL_MODE Convert(FillMode value);
        //@―---------------------------------------------------------------------------
        //! @brief  CullMode を D3D12_CULL_MODE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_CULL_MODE Convert(CullMode value);
        //@―---------------------------------------------------------------------------
        //! @brief  TextureFormat を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        static DXGI_FORMAT Convert(TextureFormat value);
        //@―---------------------------------------------------------------------------
        //! @brief  BlendFactor を D3D12_BLEND に変換
        //@―---------------------------------------------------------------------------
        static D3D12_BLEND Convert(BlendFactor value);
        //@―---------------------------------------------------------------------------
        //! @brief  BlendOp を D3D12_BLEND_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_BLEND_OP Convert(BlendOp value);
        //@―---------------------------------------------------------------------------
        //! @brief  BlendFactor を UINT8 に変換
        //@―---------------------------------------------------------------------------
        static UINT8 Convert(ColorMask value);
        //@―---------------------------------------------------------------------------
        //! @brief  LogicOp を D3D12_LOGIC_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_LOGIC_OP Convert(LogicOp value);
        //@―---------------------------------------------------------------------------
        //! @brief  Topology を D3D12_PRIMITIVE_TOPOLOGY に変換
        //@―---------------------------------------------------------------------------
        static D3D12_PRIMITIVE_TOPOLOGY Convert(Topology value);
        //@―---------------------------------------------------------------------------
        //! @brief  Type を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        static DXGI_FORMAT Convert(Type value,s32 dimension);
        //@―---------------------------------------------------------------------------
        //! @brief  Semantic を LPCSTR に変換
        //@―---------------------------------------------------------------------------
        static LPCSTR Convert(Semantic value);
        //@―---------------------------------------------------------------------------
        //! @brief  StencilOp を D3D12_STENCIL_OP に変換
        //@―---------------------------------------------------------------------------
        static D3D12_STENCIL_OP Convert(StencilOp value);
        //@―---------------------------------------------------------------------------
        //! @brief  ComparisonFunc を D3D12_COMPARISON_FUNC に変換
        //@―---------------------------------------------------------------------------
        static D3D12_COMPARISON_FUNC Convert(ComparisonFunc value);
        //@―---------------------------------------------------------------------------
        //! @brief  DescriptorHeapType を D3D12_DESCRIPTOR_HEAP_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_DESCRIPTOR_HEAP_TYPE Convert(DescriptorHeapType value);
        //@―---------------------------------------------------------------------------
        //! @brief  CommandListType を D3D12_COMMAND_LIST_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_COMMAND_LIST_TYPE Convert(CommandListType value);
        //@―---------------------------------------------------------------------------
        //! @brief  ResourceUsage を D3D12_HEAP_TYPE に変換
        //@―---------------------------------------------------------------------------
        static D3D12_HEAP_TYPE Convert(ResourceUsage value);

    };
}// namespcae ob
//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#include "TypeConverter.h"

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
        case RootParameterType::CBV:                return D3D12_ROOT_PARAMETER_TYPE_CBV;
        case RootParameterType::SRV:                return D3D12_ROOT_PARAMETER_TYPE_SRV;
        case RootParameterType::UAV:                return D3D12_ROOT_PARAMETER_TYPE_UAV;
        case RootParameterType::Range:              return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        case RootParameterType::RootConstants:      return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        }
        LOG_WARNING_EX("Graphic", "不正なRootParameterType[value={}]", enum_cast(value));
        return D3D12_ROOT_PARAMETER_TYPE_CBV;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RootParameterType を D3D12_DESCRIPTOR_RANGE_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_DESCRIPTOR_RANGE_TYPE TypeConverter::convert(DescriptorRangeType value) {
        switch (value) {
        case DescriptorRangeType::CBV:              return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        case DescriptorRangeType::SRV:              return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        case DescriptorRangeType::UAV:              return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        case DescriptorRangeType::Sampler:          return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
        }
        LOG_WARNING_EX("Graphic", "不正なDescriptorRangeType[value={}]", enum_cast(value));
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ShaderStages を D3D12_SHADER_VISIBILITY に変換
    //@―---------------------------------------------------------------------------
    D3D12_SHADER_VISIBILITY TypeConverter::convert(ShaderStage value) {
        switch (value) {
        case ShaderStage::Vertex:           return D3D12_SHADER_VISIBILITY_VERTEX;
        case ShaderStage::Hull:             return D3D12_SHADER_VISIBILITY_HULL;
        case ShaderStage::Domain:           return D3D12_SHADER_VISIBILITY_DOMAIN;
        case ShaderStage::Geometry:         return D3D12_SHADER_VISIBILITY_GEOMETRY;
        case ShaderStage::Pixel:            return D3D12_SHADER_VISIBILITY_PIXEL;
        case ShaderStage::Amplification:    return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
        case ShaderStage::Mesh:             return D3D12_SHADER_VISIBILITY_MESH;
        case ShaderStage::All:              return D3D12_SHADER_VISIBILITY_ALL;
        }

        LOG_WARNING_EX("Graphic", "不正なShaderStage[value={}]", enum_cast(value));
        return D3D12_SHADER_VISIBILITY_ALL;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  TextureFillter を D3D12_FILTER_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_FILTER_TYPE TypeConverter::convert(TextureFillter value){
        switch (value) {
        case TextureFillter::Point:         return D3D12_FILTER_TYPE_POINT;
        case TextureFillter::Linear:        return D3D12_FILTER_TYPE_LINEAR;
        }

        LOG_WARNING_EX("Graphic", "不正なTextureFillter[value={}]", enum_cast(value));
        return D3D12_FILTER_TYPE_LINEAR;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  MipFillter を D3D12_DESCRIPTOR_RANGE_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_FILTER_TYPE TypeConverter::convert(MipFillter value){
        switch (value) {
        case MipFillter::Point:             return D3D12_FILTER_TYPE_POINT;
        case MipFillter::Linear:            return D3D12_FILTER_TYPE_LINEAR;
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
        case Anisotropy::None:      return 0;
        case Anisotropy::Level1:    return 1;
        case Anisotropy::Level2:    return 2;
        case Anisotropy::Level4:    return 4;
        case Anisotropy::Level8:    return 8;
        case Anisotropy::Level16:   return 16;
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
        case TextureAddress::Repeat:   return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        case TextureAddress::Clamp:    return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        case TextureAddress::Mirror:   return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
        }
        LOG_WARNING_EX("Graphic", "不正なAnisotropy[value={}]", enum_cast(value));
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  FillMode を D3D12_FILL_MODE に変換
    //@―---------------------------------------------------------------------------
    D3D12_FILL_MODE TypeConverter::convert(FillMode value) {
        switch (value)
        {
        case FillMode::Wireframe:       return D3D12_FILL_MODE_WIREFRAME;
        case FillMode::Solid:           return D3D12_FILL_MODE_SOLID;
        case FillMode::Point:
            LOG_WARNING_EX("Graphic", "FillMode::Pointは非対応です。");
            break;
        }
        LOG_WARNING_EX("Graphic", "不正なFillMode[value={}]", enum_cast(value));
        return D3D12_FILL_MODE_SOLID;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  CullMode を D3D12_CULL_MODE に変換
    //@―---------------------------------------------------------------------------
    D3D12_CULL_MODE TypeConverter::convert(CullMode value) {
        switch (value)
        {
        case CullMode::None:            return D3D12_CULL_MODE_NONE;
        case CullMode::Front:           return D3D12_CULL_MODE_FRONT;
        case CullMode::Back:            return D3D12_CULL_MODE_BACK;
        }
        LOG_WARNING_EX("Graphic", "不正なCullMode[value={}]", enum_cast(value));
        return D3D12_CULL_MODE_NONE;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  TextureFormat を DXGI_FORMAT に変換
    //@―---------------------------------------------------------------------------
    DXGI_FORMAT TypeConverter::convert(TextureFormat value) {
        switch (value) {
        case TextureFormat::RGBA32:         return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case TextureFormat::RGBA16:         return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case TextureFormat::RGBA8:          return DXGI_FORMAT_R8G8B8A8_UNORM;

        case TextureFormat::RGB32:          return DXGI_FORMAT_R32G32B32_FLOAT;
        case TextureFormat::RGB8:           return DXGI_FORMAT_B8G8R8X8_UNORM;

        case TextureFormat::RG32:           return DXGI_FORMAT_R32G32_FLOAT;
        case TextureFormat::RG16:           return DXGI_FORMAT_R16G16_FLOAT;
        case TextureFormat::RG8:            return DXGI_FORMAT_R8G8_UNORM;

        case TextureFormat::R32:            return DXGI_FORMAT_R32_FLOAT;
        case TextureFormat::R16:            return DXGI_FORMAT_R16_FLOAT;
        case TextureFormat::R8:             return DXGI_FORMAT_R8_UNORM;

        case TextureFormat::R10G10B10A2:    return DXGI_FORMAT_R10G10B10A2_UNORM;

        case TextureFormat::D32S8:          return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case TextureFormat::D32:            return DXGI_FORMAT_D32_FLOAT;
        case TextureFormat::D24S8:          return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormat::D16:            return DXGI_FORMAT_D16_UNORM;

        case TextureFormat::BC1:            return DXGI_FORMAT_BC1_UNORM;
        case TextureFormat::BC2:            return DXGI_FORMAT_BC2_UNORM;
        case TextureFormat::BC3:            return DXGI_FORMAT_BC3_UNORM;
        case TextureFormat::BC4:            return DXGI_FORMAT_BC4_UNORM;
        case TextureFormat::BC5:            return DXGI_FORMAT_BC5_UNORM;
        case TextureFormat::BC6H:           return DXGI_FORMAT_BC6H_UF16;
        case TextureFormat::BC7:            return DXGI_FORMAT_BC7_UNORM;

        case TextureFormat::BC1_SRGB:       return DXGI_FORMAT_BC1_UNORM_SRGB;
        case TextureFormat::BC2_SRGB:       return DXGI_FORMAT_BC2_UNORM_SRGB;
        case TextureFormat::BC3_SRGB:       return DXGI_FORMAT_BC3_UNORM_SRGB;
        case TextureFormat::BC7_SRGB:       return DXGI_FORMAT_BC7_UNORM_SRGB;
        }

        LOG_WARNING_EX("Graphic", "不正なTextureFormat[value={}]", enum_cast(value));
        return DXGI_FORMAT_UNKNOWN;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  BlendFactor を D3D12_BLEND に変換
    //@―---------------------------------------------------------------------------
    D3D12_BLEND TypeConverter::convert(BlendFactor value) {
        switch (value)
        {
        case BlendFactor::Zero:                     return D3D12_BLEND_ZERO;
        case BlendFactor::One:                      return D3D12_BLEND_ONE;
        case BlendFactor::SrcColor:                 return D3D12_BLEND_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:         return D3D12_BLEND_INV_SRC_COLOR;
        case BlendFactor::DstColor:                 return D3D12_BLEND_DEST_COLOR;
        case BlendFactor::OneMinusDstColor:         return D3D12_BLEND_INV_DEST_COLOR;
        case BlendFactor::SrcAlpha:                 return D3D12_BLEND_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:         return D3D12_BLEND_INV_SRC_ALPHA;
        case BlendFactor::DstAlpha:                 return D3D12_BLEND_DEST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:         return D3D12_BLEND_INV_DEST_ALPHA;
        }
        LOG_WARNING_EX("Graphic", "不正なBlendFactor[value={}]", enum_cast(value));
        return D3D12_BLEND_ZERO;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  BlendOp を D3D12_BLEND_OP に変換
    //@―---------------------------------------------------------------------------
    D3D12_BLEND_OP TypeConverter::convert(BlendOp value) {
        switch (value)
        {
        case BlendOp::Add:     return D3D12_BLEND_OP_ADD;
        case BlendOp::Sub:     return D3D12_BLEND_OP_SUBTRACT;
        case BlendOp::RevSub:  return D3D12_BLEND_OP_REV_SUBTRACT;
        case BlendOp::Min:     return D3D12_BLEND_OP_MIN;
        case BlendOp::Max:     return D3D12_BLEND_OP_MAX;
        }
        LOG_WARNING_EX("Graphic", "不正なBlendOp[value={}]", enum_cast(value));
        return D3D12_BLEND_OP_ADD;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  BlendFactor を UINT8 に変換
    //@―---------------------------------------------------------------------------
    UINT8 TypeConverter::convert(ColorMask value) {
        UINT8 result = 0;
        if (value[ColorCompoent::R])result |= D3D12_COLOR_WRITE_ENABLE_RED;
        if (value[ColorCompoent::G])result |= D3D12_COLOR_WRITE_ENABLE_GREEN;
        if (value[ColorCompoent::B])result |= D3D12_COLOR_WRITE_ENABLE_BLUE;
        if (value[ColorCompoent::A])result |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  LogicOp を D3D12_LOGIC_OP に変換
    //@―---------------------------------------------------------------------------
    D3D12_LOGIC_OP TypeConverter::convert(LogicOp value) {
        switch (value)
        {
        case LogicOp::Clear:           return D3D12_LOGIC_OP_CLEAR;
        case LogicOp::Set:             return D3D12_LOGIC_OP_SET;
        case LogicOp::Copy:            return D3D12_LOGIC_OP_COPY;
        case LogicOp::CopyInverted:    return D3D12_LOGIC_OP_COPY_INVERTED;
        case LogicOp::Noop:            return D3D12_LOGIC_OP_NOOP;
        case LogicOp::Invert:          return D3D12_LOGIC_OP_INVERT;
        case LogicOp::And:             return D3D12_LOGIC_OP_AND;
        case LogicOp::Nand:            return D3D12_LOGIC_OP_NAND;
        case LogicOp::Or:              return D3D12_LOGIC_OP_OR;
        case LogicOp::Nor:             return D3D12_LOGIC_OP_NOR;
        case LogicOp::Xor:             return D3D12_LOGIC_OP_XOR;
        case LogicOp::Equivalent:      return D3D12_LOGIC_OP_EQUIV;
        case LogicOp::AndReverse:      return D3D12_LOGIC_OP_AND_REVERSE;
        case LogicOp::AndInverted:     return D3D12_LOGIC_OP_AND_INVERTED;
        case LogicOp::OrReverse:       return D3D12_LOGIC_OP_OR_REVERSE;
        case LogicOp::OrInverted:      return D3D12_LOGIC_OP_OR_INVERTED;
        }
        LOG_WARNING_EX("Graphic", "不正なLogicOp[value={}]", enum_cast(value));
        return D3D12_LOGIC_OP_CLEAR;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Topology を D3D12_PRIMITIVE_TOPOLOGY に変換
    //@―---------------------------------------------------------------------------
    D3D12_PRIMITIVE_TOPOLOGY TypeConverter::convert(Topology value) {
        switch (value)
        {
        case Topology::PointList:      return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
        case Topology::LineList:       return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        case Topology::TriangleList:   return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        }
        LOG_WARNING_EX("Graphic", "不正なTopology[value={}]", enum_cast(value));
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Type を DXGI_FORMAT に変換
    //@―---------------------------------------------------------------------------
    DXGI_FORMAT TypeConverter::convert(Type value, s32 dimension) {
        if (dimension == 1) {
            switch (value)
            {
            case Type::Int8:   return DXGI_FORMAT_R8_SINT;
            case Type::Int16:  return DXGI_FORMAT_R16_SINT;
            case Type::Int32:  return DXGI_FORMAT_R32_SINT;
            case Type::UInt8:  return DXGI_FORMAT_R8_UINT;
            case Type::UInt16: return DXGI_FORMAT_R16_UINT;
            case Type::UInt32: return DXGI_FORMAT_R32_UINT;
            case Type::Float:  return DXGI_FORMAT_R32_FLOAT;
            }
        }
        if (dimension == 2) {
            switch (value)
            {
            case Type::Int8:   return DXGI_FORMAT_R8G8_SINT;
            case Type::Int16:  return DXGI_FORMAT_R16G16_SINT;
            case Type::Int32:  return DXGI_FORMAT_R32G32_SINT;
            case Type::UInt8:  return DXGI_FORMAT_R8G8_UINT;
            case Type::UInt16: return DXGI_FORMAT_R16G16_UINT;
            case Type::UInt32: return DXGI_FORMAT_R32G32_UINT;
            case Type::Float:  return DXGI_FORMAT_R32G32_FLOAT;
            }
        }
        if (dimension == 3) {
            switch (value)
            {
            case Type::Int8:   return DXGI_FORMAT_R8G8B8A8_SINT;//
            case Type::Int16:  return DXGI_FORMAT_R16G16B16A16_SINT;//
            case Type::Int32:  return DXGI_FORMAT_R32G32B32_SINT;
            case Type::UInt8:  return DXGI_FORMAT_R8G8B8A8_UINT;//
            case Type::UInt16: return DXGI_FORMAT_R16G16B16A16_UINT;//
            case Type::UInt32: return DXGI_FORMAT_R32G32B32_UINT;
            case Type::Float:  return DXGI_FORMAT_R32G32B32_FLOAT;
            }
        }
        if (dimension == 3 || dimension == 4) {
            switch (value)
            {
            case Type::Int8:   return DXGI_FORMAT_R8G8B8A8_SINT;
            case Type::Int16:  return DXGI_FORMAT_R16G16B16A16_SINT;
            case Type::Int32:  return DXGI_FORMAT_R32G32B32A32_SINT;
            case Type::UInt8:  return DXGI_FORMAT_R8G8B8A8_UINT;
            case Type::UInt16: return DXGI_FORMAT_R16G16B16A16_UINT;
            case Type::UInt32: return DXGI_FORMAT_R32G32B32A32_UINT;
            case Type::Float:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
            }
        }
        LOG_WARNING_EX("Graphic", "不正なSemantic[value={0},index={1}]", enum_cast(value),dimension);
        return DXGI_FORMAT_UNKNOWN;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Semantic を LPCSTR に変換
    //@―---------------------------------------------------------------------------
    LPCSTR TypeConverter::convert(Semantic value) {
        switch (value)
        {
        case Semantic::Position:       return "POSITION";
        case Semantic::Normal:         return "NORMAL";
        case Semantic::Binormal:       return "BINOMAL";
        case Semantic::Tangent:        return "TANGENT";
        case Semantic::Color:          return "COLOR";
        case Semantic::TexCoord:       return "TEXCOORD";
        case Semantic::BlendIndices:   return "BLENDINDICES";
        case Semantic::BlendWeights:   return "BLENDWEIGHT";
        case Semantic::PointSize:      return "PSIZE";
        }
        LOG_WARNING_EX("Graphic", "不正なSemantic[value={}]", enum_cast(value));
        return "";
    }


    //@―---------------------------------------------------------------------------
    //! @brief  StencilOp を D3D12_STENCIL_OP に変換
    //@―---------------------------------------------------------------------------
    D3D12_STENCIL_OP TypeConverter::convert(StencilOp value) {
        switch (value)
        {
        case StencilOp::Keep:              return D3D12_STENCIL_OP_KEEP;
        case StencilOp::Zero:              return D3D12_STENCIL_OP_ZERO;
        case StencilOp::Replace:           return D3D12_STENCIL_OP_REPLACE;
        case StencilOp::IncrementAndClamp: return D3D12_STENCIL_OP_INCR_SAT;
        case StencilOp::DecrementAndClamp: return D3D12_STENCIL_OP_DECR_SAT;
        case StencilOp::Invert:            return D3D12_STENCIL_OP_INVERT;
        case StencilOp::IncrementAndWrap:  return D3D12_STENCIL_OP_INCR;
        case StencilOp::DecrementAndWrap:  return D3D12_STENCIL_OP_DECR;
        }
        LOG_WARNING_EX("Graphic", "不正なStencilOp[value={}]", enum_cast(value));
        return D3D12_STENCIL_OP_KEEP;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ComparisonFunc を D3D12_COMPARISON_FUNC に変換
    //@―---------------------------------------------------------------------------
    D3D12_COMPARISON_FUNC TypeConverter::convert(ComparisonFunc value) {
        switch (value)
        {
        case ComparisonFunc::Never:        return D3D12_COMPARISON_FUNC_NEVER;
        case ComparisonFunc::Always:       return D3D12_COMPARISON_FUNC_ALWAYS;
        case ComparisonFunc::Equal:        return D3D12_COMPARISON_FUNC_EQUAL;
        case ComparisonFunc::NotEqual:     return D3D12_COMPARISON_FUNC_NOT_EQUAL;
        case ComparisonFunc::Less:         return D3D12_COMPARISON_FUNC_LESS;
        case ComparisonFunc::Greater:      return D3D12_COMPARISON_FUNC_GREATER;
        case ComparisonFunc::LessEqual:    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case ComparisonFunc::GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        }
        LOG_WARNING_EX("Graphic", "不正なComparisonFunc[value={}]", enum_cast(value));
        return D3D12_COMPARISON_FUNC_NEVER;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  DescriptorHeapType を D3D12_DESCRIPTOR_HEAP_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_DESCRIPTOR_HEAP_TYPE TypeConverter::convert(DescriptorHeapType value) {
        switch (value)
        {
        case DescriptorHeapType::CBV_SRV_UAV:  return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        case DescriptorHeapType::Sampler:      return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        case DescriptorHeapType::RTV:          return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        case DescriptorHeapType::DSV:          return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        }

        LOG_WARNING_EX("Graphic", "不正なDescriptorHeapType[value={}]", enum_cast(value));
        return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  CommandListType を D3D12_COMMAND_LIST_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_COMMAND_LIST_TYPE TypeConverter::convert(CommandListType value) {
        switch (value)
        {
        case CommandListType::Graphic: return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case CommandListType::Compute: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        }

        LOG_WARNING_EX("Graphic", "不正なCommandListType[value={}]", enum_cast(value));
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ResourceUsage を D3D12_HEAP_TYPE に変換
    //@―---------------------------------------------------------------------------
    D3D12_HEAP_TYPE TypeConverter::convert(ResourceUsage value) {
        switch (value)
        {
        case ResourceUsage::Default: return D3D12_HEAP_TYPE_DEFAULT;
        //case ResourceUsage::Immutable: return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case ResourceUsage::Dynamic: return D3D12_HEAP_TYPE_UPLOAD;
        case ResourceUsage::ReadBack: return D3D12_HEAP_TYPE_DEFAULT;
        //case ResourceUsage::WiteBack: return D3D12_COMMAND_LIST_TYPE_DIRECT;
        }

        LOG_WARNING_EX("Graphic", "不正なResourceUsage[value={}]", enum_cast(value));
        return D3D12_HEAP_TYPE_DEFAULT;
    }

}// namespace ob
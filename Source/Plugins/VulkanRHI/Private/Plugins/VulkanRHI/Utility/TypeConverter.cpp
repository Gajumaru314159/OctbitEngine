#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi {

	vk::ImageType TypeConverter::Convert(TextureType value) {
		switch (value) {
		case TextureType::Texture1D:		return vk::ImageType::e1D;
		case TextureType::Texture2D:		return vk::ImageType::e2D;
		case TextureType::Texture3D:		return vk::ImageType::e3D;
		case TextureType::Cube:				return vk::ImageType::e2D;
		}
		throw NotSupportedException();
	}

	vk::AccessFlags TypeConverter::Convert(BufferState state) {
		switch (state)
		{
		case BufferState::Common:					return vk::AccessFlagBits::eNone;
		case BufferState::Vertex:				return vk::AccessFlagBits::eVertexAttributeRead;
		case BufferState::Index:				return vk::AccessFlagBits::eIndexRead;
		case BufferState::Constant:			return vk::AccessFlagBits::eUniformRead;
		case BufferState::ShaderResource:			return vk::AccessFlagBits::eShaderRead;
		case BufferState::UnorderedAccess:			return vk::AccessFlagBits::eShaderWrite;
		case BufferState::IndirectArgument:			return vk::AccessFlagBits::eIndirectCommandRead;
		}
		throw NotSupportedException();
	}

	vk::PrimitiveTopology TypeConverter::Convert(Topology value) {
		switch (value) {
		case Topology::PointList:		return vk::PrimitiveTopology::ePointList;
		case Topology::LineList:		return vk::PrimitiveTopology::eLineList;
		case Topology::TriangleList:	return vk::PrimitiveTopology::eTriangleList;
		}
		throw NotSupportedException();
	}
	vk::Format TypeConverter::Convert(ElementType type, s32 dimension) {
		switch (type) {
		case ElementType::Int8:
			switch (dimension) {
			case 1: return vk::Format::eR8Sint;
			case 2: return vk::Format::eR8G8Sint;
			case 3: return vk::Format::eR8G8B8Sint;
			case 4: return vk::Format::eR8G8B8A8Sint;
			}
			break;
		case ElementType::Int16:
			switch (dimension) {
			case 1: return vk::Format::eR16Sint;
			case 2: return vk::Format::eR16G16Sint;
			case 3: return vk::Format::eR16G16B16Sint;
			case 4: return vk::Format::eR16G16B16A16Sint;
			}
			break;
		case ElementType::Int32:
			switch (dimension) {
			case 1: return vk::Format::eR32Sint;
			case 2: return vk::Format::eR32G32Sint;
			case 3: return vk::Format::eR32G32B32Sint;
			case 4: return vk::Format::eR32G32B32A32Sint;
			}
			break;
		case ElementType::UInt8:
			switch (dimension) {
			case 1: return vk::Format::eR8Uint;
			case 2: return vk::Format::eR8G8Uint;
			case 3: return vk::Format::eR8G8B8Uint;
			case 4: return vk::Format::eR8G8B8A8Uint;
			}
			break;
		case ElementType::UInt16:
			switch (dimension) {
			case 1: return vk::Format::eR16Uint;
			case 2: return vk::Format::eR16G16Uint;
			case 3: return vk::Format::eR16G16B16Uint;
			case 4: return vk::Format::eR16G16B16A16Uint;
			}
			break;
		case ElementType::UInt32:
			switch (dimension) {
			case 1: return vk::Format::eR32Uint;
			case 2: return vk::Format::eR32G32Uint;
			case 3: return vk::Format::eR32G32B32Uint;
			case 4: return vk::Format::eR32G32B32A32Uint;
			}
			break;
		case ElementType::Float:
			switch (dimension) {
			case 1: return vk::Format::eR32Sfloat;
			case 2: return vk::Format::eR32G32Sfloat;
			case 3: return vk::Format::eR32G32B32Sfloat;
			case 4: return vk::Format::eR32G32B32A32Sfloat;
			}
			break;
		case ElementType::Int8Norm:
			switch (dimension) {
			case 1: return vk::Format::eR8Snorm;
			case 2: return vk::Format::eR8G8Snorm;
			case 3: return vk::Format::eR8G8B8Snorm;
			case 4: return vk::Format::eR8G8B8A8Snorm;
			}
			break;
		case ElementType::UInt8Norm:
			switch (dimension) {
			case 1: return vk::Format::eR8Unorm;
			case 2: return vk::Format::eR8G8Unorm;
			case 3: return vk::Format::eR8G8B8Unorm;
			case 4: return vk::Format::eR8G8B8A8Unorm;
			}
			break;
		}
		throw NotSupportedException();
	}
	s32 TypeConverter::Convert(VertexInputRate value) {
		switch (value) {
		case VertexInputRate::Vertex:	return 0;
		case VertexInputRate::Instance: return 1;
		}
		throw NotSupportedException();
	}

	vk::PolygonMode TypeConverter::Convert(FillMode value) {
		switch (value) {
		case FillMode::Wireframe:	return vk::PolygonMode::eLine;
		case FillMode::Solid:		return vk::PolygonMode::eFill;
		}
		throw NotSupportedException();
	}
	vk::CullModeFlags TypeConverter::Convert(CullMode value) {
		switch (value) {
		case CullMode::None:	return vk::CullModeFlagBits::eNone;
		case CullMode::Front:	return vk::CullModeFlagBits::eFront;
		case CullMode::Back:	return vk::CullModeFlagBits::eBack;
		}
		throw NotSupportedException();
	}
	vk::CompareOp TypeConverter::Convert(ComparisonFunc value) {
		switch (value) {
		case ComparisonFunc::Never:			return vk::CompareOp::eNever;
		case ComparisonFunc::Less:			return vk::CompareOp::eLess;
		case ComparisonFunc::Equal:			return vk::CompareOp::eEqual;
		case ComparisonFunc::LessEqual:		return vk::CompareOp::eLessOrEqual;
		case ComparisonFunc::Greater:		return vk::CompareOp::eGreater;
		case ComparisonFunc::NotEqual:		return vk::CompareOp::eNotEqual;
		case ComparisonFunc::GreaterEqual:	return vk::CompareOp::eGreaterOrEqual;
		case ComparisonFunc::Always:		return vk::CompareOp::eAlways;
		}
		throw NotSupportedException();
	}
	vk::BlendFactor TypeConverter::Convert(BlendFactor value) {
		switch (value) {
		case BlendFactor::Zero:				return vk::BlendFactor::eZero;
		case BlendFactor::One:				return vk::BlendFactor::eOne;
		case BlendFactor::SrcColor:			return vk::BlendFactor::eSrcColor;
		case BlendFactor::OneMinusSrcColor: return vk::BlendFactor::eOneMinusSrcColor;
		case BlendFactor::SrcAlpha:			return vk::BlendFactor::eSrcAlpha;
		case BlendFactor::OneMinusSrcAlpha: return vk::BlendFactor::eOneMinusSrcAlpha;
		case BlendFactor::DstAlpha:			return vk::BlendFactor::eDstAlpha;
		case BlendFactor::OneMinusDstAlpha: return vk::BlendFactor::eOneMinusDstAlpha;
		case BlendFactor::DstColor:			return vk::BlendFactor::eDstColor;
		case BlendFactor::OneMinusDstColor: return vk::BlendFactor::eOneMinusDstColor;
		}
		throw NotSupportedException();
	}
	vk::BlendOp TypeConverter::Convert(BlendOp value) {
		switch (value) {
		case BlendOp::Add:					return vk::BlendOp::eAdd;
		case BlendOp::Sub:					return vk::BlendOp::eSubtract;
		case BlendOp::RevSub:				return vk::BlendOp::eReverseSubtract;
		case BlendOp::Min:					return vk::BlendOp::eMin;
		case BlendOp::Max:					return vk::BlendOp::eMax;
		}
		throw NotSupportedException();
	}
	vk::StencilOp TypeConverter::Convert(StencilOp value) {
		switch (value) {
		case StencilOp::Keep:				return vk::StencilOp::eKeep;
		case StencilOp::Replace:			return vk::StencilOp::eReplace;
		case StencilOp::Zero:				return vk::StencilOp::eZero;
		case StencilOp::Invert:				return vk::StencilOp::eInvert;
		case StencilOp::IncrementAndClamp:	return vk::StencilOp::eIncrementAndClamp;
		case StencilOp::DecrementAndClamp:	return vk::StencilOp::eDecrementAndClamp;
		case StencilOp::IncrementAndWrap:	return vk::StencilOp::eIncrementAndWrap;
		case StencilOp::DecrementAndWrap:	return vk::StencilOp::eDecrementAndWrap;
		}
		throw NotSupportedException();
	}
	vk::ColorComponentFlags TypeConverter::Convert(ColorMask value) {
		vk::ColorComponentFlags result{ 0 };
		if (value & ColorComponent::R) result |= vk::ColorComponentFlagBits::eR;
		if (value & ColorComponent::G) result |= vk::ColorComponentFlagBits::eG;
		if (value & ColorComponent::B) result |= vk::ColorComponentFlagBits::eB;
		if (value & ColorComponent::A) result |= vk::ColorComponentFlagBits::eA;
		return result;
	}

	// TODO 複数ファイルで使用するので、共通化する
	vk::Format TypeConverter::Convert(TextureFormat value) {
		switch (value) {
		case TextureFormat::RGBA32:         return vk::Format::eR32G32B32A32Sfloat;
		case TextureFormat::RGBA16:         return vk::Format::eR16G16B16A16Sfloat;
		case TextureFormat::RGBA8:          return vk::Format::eR8G8B8A8Unorm;

		case TextureFormat::RGBA8_SRGB:     return vk::Format::eR8G8B8A8Srgb;


		case TextureFormat::RGB32:          return vk::Format::eR32G32B32Sfloat;
		case TextureFormat::RGB8:           return vk::Format::eR8G8B8Unorm;

		case TextureFormat::RG32:           return vk::Format::eR32G32Sfloat;
		case TextureFormat::RG16:           return vk::Format::eR16G16Sfloat;
		case TextureFormat::RG8:            return vk::Format::eR8G8Unorm;

		case TextureFormat::R32:            return vk::Format::eR32Sfloat;
		case TextureFormat::R16:            return vk::Format::eR16Sfloat;
		case TextureFormat::R8:             return vk::Format::eR8Unorm;

		case TextureFormat::R10G10B10A2:    return vk::Format::eA2R10G10B10UnormPack32;

		case TextureFormat::D32S8:          return vk::Format::eD32SfloatS8Uint;
		case TextureFormat::D32:            return vk::Format::eD32Sfloat;
		case TextureFormat::D24S8:          return vk::Format::eD24UnormS8Uint;
		case TextureFormat::D16:            return vk::Format::eD16Unorm;

		case TextureFormat::BC1:            return vk::Format::eBc1RgbaUnormBlock;
		case TextureFormat::BC2:            return vk::Format::eBc2UnormBlock;
		case TextureFormat::BC3:            return vk::Format::eBc3UnormBlock;
		case TextureFormat::BC4:            return vk::Format::eBc4UnormBlock;
		case TextureFormat::BC5:            return vk::Format::eBc5UnormBlock;
		case TextureFormat::BC6H:           return vk::Format::eBc6HSfloatBlock;
		case TextureFormat::BC7:            return vk::Format::eBc7UnormBlock;

		case TextureFormat::BC1_SRGB:       return vk::Format::eBc1RgbaSrgbBlock;
		case TextureFormat::BC2_SRGB:       return vk::Format::eBc2SrgbBlock;
		case TextureFormat::BC3_SRGB:       return vk::Format::eBc3SrgbBlock;
		case TextureFormat::BC7_SRGB:       return vk::Format::eBc7SrgbBlock;
		}
		throw NotSupportedException();
	}

	vk::ImageLayout TypeConverter::Convert(TextureState value) {
		switch (value) {
		case TextureState::Common:			return vk::ImageLayout::eUndefined;
		case TextureState::ShaderResource:	return vk::ImageLayout::eShaderReadOnlyOptimal;
		case TextureState::UnorderedAccess:	return vk::ImageLayout::eGeneral;
		case TextureState::RenderTarget:	return vk::ImageLayout::eColorAttachmentOptimal;
		case TextureState::DepthRead:		return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
		case TextureState::DepthWrite:		return vk::ImageLayout::eDepthStencilAttachmentOptimal;
		case TextureState::CopySource:		return vk::ImageLayout::eTransferSrcOptimal;
		case TextureState::CopyDest:		return vk::ImageLayout::eTransferDstOptimal;
		case TextureState::Present:			return vk::ImageLayout::ePresentSrcKHR;
		}
		throw NotSupportedException();
	}

	vk::DescriptorType TypeConverter::Convert(BindingType value) {
		switch (value)
		{
		case BindingType::Texture:
			return vk::DescriptorType::eSampledImage;

		case BindingType::RWTexture:
			return vk::DescriptorType::eStorageImage;

		case BindingType::Buffer:
			return vk::DescriptorType::eUniformTexelBuffer;

		case BindingType::RWBuffer:
			return vk::DescriptorType::eStorageTexelBuffer;

		case BindingType::StructuredBuffer:
		case BindingType::RWStructuredBuffer:
		case BindingType::ByteAddressBuffer:
		case BindingType::RWByteAddressBuffer:
			return vk::DescriptorType::eStorageBuffer;

		case BindingType::ConstantBuffer:
			return vk::DescriptorType::eUniformBuffer;

		case BindingType::Sampler:
			return vk::DescriptorType::eSampler;
		}
		throw NotSupportedException();
	}

	vk::Filter TypeConverter::Convert(TextureFilter filter) {
		switch (filter) {
		case TextureFilter::Point: return vk::Filter::eNearest;
		case TextureFilter::Linear: return vk::Filter::eLinear;
		default: return vk::Filter::eLinear;
		}
	}

	vk::SamplerMipmapMode TypeConverter::Convert(MipFilter mipFilter) {
		switch (mipFilter) {
		case MipFilter::Point: return vk::SamplerMipmapMode::eNearest;
		case MipFilter::Linear: return vk::SamplerMipmapMode::eLinear;
		default: return vk::SamplerMipmapMode::eLinear;
		}
	}

	vk::SamplerAddressMode TypeConverter::Convert(TextureAddress address) {
		switch (address) {
		case TextureAddress::Repeat: return vk::SamplerAddressMode::eRepeat;
		case TextureAddress::Clamp: return vk::SamplerAddressMode::eClampToEdge;
		case TextureAddress::Mirror: return vk::SamplerAddressMode::eMirroredRepeat;
		default: return vk::SamplerAddressMode::eRepeat;
		}
	}

	f32 TypeConverter::Convert(Anisotropy anisotropy) {
		switch (anisotropy) {
		case Anisotropy::None: return 0;
		case Anisotropy::Level1: return 1;
		case Anisotropy::Level2: return 2;
		case Anisotropy::Level4: return 4;
		case Anisotropy::Level8: return 8;
		case Anisotropy::Level16: return 16;
		default: return 0;
		}
	}

	vk::AttachmentLoadOp TypeConverter::Convert(RenderPassBeforeAccessType type) {
		switch (type) {
		case RenderPassBeforeAccessType::Discard: return vk::AttachmentLoadOp::eClear;
		case RenderPassBeforeAccessType::Preserve: return vk::AttachmentLoadOp::eLoad;
		case RenderPassBeforeAccessType::Clear: return vk::AttachmentLoadOp::eClear;
		case RenderPassBeforeAccessType::NoAccess: return vk::AttachmentLoadOp::eDontCare;
		}
		throw NotSupportedException();
	}

	vk::AttachmentStoreOp TypeConverter::Convert(RenderPassAfterAccessType type) {
		switch (type) {
		case RenderPassAfterAccessType::Discard: return vk::AttachmentStoreOp::eDontCare;
		case RenderPassAfterAccessType::Preserve: return vk::AttachmentStoreOp::eStore;
		case RenderPassAfterAccessType::NoAccess: return vk::AttachmentStoreOp::eDontCare;
		}
		throw NotSupportedException();
	}


	TextureFormat TypeConverter::Convert(vk::Format value) {
		switch (value) {
		case vk::Format::eR8G8B8A8Unorm: return TextureFormat::RGBA8;
		case vk::Format::eB8G8R8A8Unorm: return TextureFormat::RGBA8;
		case vk::Format::eR8G8B8A8Srgb: return TextureFormat::RGBA8_SRGB;
		case vk::Format::eR32G32B32A32Sfloat: return TextureFormat::RGBA32;
		case vk::Format::eR16G16B16A16Sfloat: return TextureFormat::RGBA16;
		case vk::Format::eR8G8B8Unorm: return TextureFormat::RGB8;
		case vk::Format::eR32G32B32Sfloat: return TextureFormat::RGB32;
		case vk::Format::eR16G16B16A16Uint: return TextureFormat::RGBA16;
		case vk::Format::eD32SfloatS8Uint: return TextureFormat::D32S8;
		case vk::Format::eD24UnormS8Uint: return TextureFormat::D24S8;
		case vk::Format::eD32Sfloat: return TextureFormat::D32;
		case vk::Format::eD16Unorm: return TextureFormat::D16;
		case vk::Format::eBc1RgbaUnormBlock: return TextureFormat::BC1;
		case vk::Format::eBc2UnormBlock: return TextureFormat::BC2;
		case vk::Format::eBc3UnormBlock: return TextureFormat::BC3;
		case vk::Format::eBc4UnormBlock: return TextureFormat::BC4;
		case vk::Format::eBc5UnormBlock: return TextureFormat::BC5;
		case vk::Format::eBc6HSfloatBlock: return TextureFormat::BC6H;
		case vk::Format::eBc7UnormBlock: return TextureFormat::BC7;
		}
		throw NotSupportedException();
	}

}
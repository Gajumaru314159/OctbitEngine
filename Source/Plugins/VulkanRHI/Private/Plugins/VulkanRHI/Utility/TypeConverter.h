//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/RHI/Types/RootSignatureDesc.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>
#include <Framework/RHI/Types/DescriptorDesc.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <Framework/RHI/RenderPass.h>

namespace ob::rhi::vulkan {

	class TypeConverter {
	public:
		static vk::ImageType Convert(TextureType value);
		static vk::AccessFlags Convert(BufferState state);
		static vk::PrimitiveTopology Convert(Topology value);
		static vk::Format Convert(ElementType type, s32 dimension);
		static s32 Convert(VertexInputRate value);
		static vk::PolygonMode Convert(FillMode value);
		static vk::CullModeFlags Convert(CullMode value);
		static vk::CompareOp Convert(ComparisonFunc value);
		static vk::BlendFactor Convert(BlendFactor value);
		static vk::BlendOp Convert(BlendOp value);
		static vk::StencilOp Convert(StencilOp value);
		static vk::ColorComponentFlags Convert(ColorMask value);
		static vk::Format Convert(TextureFormat value);
		static vk::ImageLayout Convert(TextureState value);
		static  vk::DescriptorType Convert(BindingType value);
		static vk::Filter Convert(TextureFillter filter);
		static vk::SamplerMipmapMode Convert(MipFillter mipFilter);
		static vk::SamplerAddressMode Convert(TextureAddress address);
		static f32 Convert(Anisotropy anisotropy);
		static vk::AttachmentLoadOp Convert(RenderPassBeforeAccessType type);
		static vk::AttachmentStoreOp Convert(RenderPassAfterAccessType type);

		static TextureFormat Convert(vk::Format value);
	};

}
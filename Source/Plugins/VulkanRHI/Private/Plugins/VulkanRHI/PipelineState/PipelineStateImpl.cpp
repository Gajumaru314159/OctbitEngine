//***********************************************************
//! @file
//! @brief		パイプラインステート実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
//#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/VulkanRHI/Shader/ShaderImpl.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>

namespace ob::rhi::vulkan {

    static vk::PrimitiveTopology Convert(Topology value) {
        switch (value) {
        case Topology::PointList:
            return vk::PrimitiveTopology::ePointList;
        case Topology::LineList:
            return vk::PrimitiveTopology::eLineList;
        case Topology::TriangleList:
            return vk::PrimitiveTopology::eTriangleList;
        default:
            throw std::invalid_argument("Unsupported Topology value");
        }
    }
    static vk::Format Convert(ElementType type,s32 dimension) {
        switch (type) {
		case ElementType::Int8:
			switch (dimension) {
			case 1: return vk::Format::eR8Sint;
			case 2: return vk::Format::eR8G8Sint;
			case 3: return vk::Format::eR8G8B8Sint;
			case 4: return vk::Format::eR8G8B8A8Sint;
			}
		case ElementType::Int16:
			switch (dimension) {
			case 1: return vk::Format::eR16Sint;
			case 2: return vk::Format::eR16G16Sint;
			case 3: return vk::Format::eR16G16B16Sint;
			case 4: return vk::Format::eR16G16B16A16Sint;
			}
		case ElementType::Int32:
			switch (dimension) {
			case 1: return vk::Format::eR32Sint;
			case 2: return vk::Format::eR32G32Sint;
			case 3: return vk::Format::eR32G32B32Sint;
			case 4: return vk::Format::eR32G32B32A32Sint;
			}
		case ElementType::UInt8:
			switch (dimension) {
			case 1: return vk::Format::eR8Uint;
			case 2: return vk::Format::eR8G8Uint;
			case 3: return vk::Format::eR8G8B8Uint;
			case 4: return vk::Format::eR8G8B8A8Uint;
			}
		case ElementType::UInt16:
			switch (dimension) {
			case 1: return vk::Format::eR16Uint;
			case 2: return vk::Format::eR16G16Uint;
			case 3: return vk::Format::eR16G16B16Uint;
			case 4: return vk::Format::eR16G16B16A16Uint;
			}
		case ElementType::UInt32:
			switch (dimension) {
			case 1: return vk::Format::eR32Uint;
			case 2: return vk::Format::eR32G32Uint;
			case 3: return vk::Format::eR32G32B32Uint;
			case 4: return vk::Format::eR32G32B32A32Uint;
			}
		case ElementType::Float:
			switch (dimension) {
			case 1: return vk::Format::eR32Sfloat;
			case 2: return vk::Format::eR32G32Sfloat;
			case 3: return vk::Format::eR32G32B32Sfloat;
			case 4: return vk::Format::eR32G32B32A32Sfloat;
			}
        }
		throw std::invalid_argument("Unsupported Topology value");
    }
	static s32 Convert(VertexInputRate value) {
		switch (value) {
		case VertexInputRate::Vertex: return 0;
		case VertexInputRate::Instance: return 1;
		}
		throw std::invalid_argument("Unsupported Topology value");
	}

	static vk::PolygonMode Convert(FillMode value) {
		switch (value) {
		case FillMode::Wireframe: return vk::PolygonMode::eLine;
		case FillMode::Solid: return vk::PolygonMode::eFill;
		}
		throw std::invalid_argument("Unsupported Topology value");
	}
	static vk::CullModeFlags Convert(CullMode value) {
		switch (value) {
		case CullMode::None: return vk::CullModeFlagBits::eNone;
		case CullMode::Front: return vk::CullModeFlagBits::eFront;
		case CullMode::Back: return vk::CullModeFlagBits::eBack;
		}
		throw std::invalid_argument("Unsupported Topology value");
	}
	static vk::CompareOp Convert(ComparisonFunc value) {
		switch (value) {
		case ComparisonFunc::Never: return vk::CompareOp::eNever;
		case ComparisonFunc::Less: return vk::CompareOp::eLess;
		case ComparisonFunc::Equal: return vk::CompareOp::eEqual;
		case ComparisonFunc::LessEqual: return vk::CompareOp::eLessOrEqual;
		case ComparisonFunc::Greater: return vk::CompareOp::eGreater;
		case ComparisonFunc::NotEqual: return vk::CompareOp::eNotEqual;
		case ComparisonFunc::GreaterEqual: return vk::CompareOp::eGreaterOrEqual;
		case ComparisonFunc::Always: return vk::CompareOp::eAlways;
		}
		throw std::invalid_argument("Unsupported Topology value");
	}
	static vk::BlendFactor Convert(BlendFactor value) {
		switch (value) {
		case BlendFactor::Zero: return vk::BlendFactor::eZero;
		case BlendFactor::One: return vk::BlendFactor::eOne;
		case BlendFactor::SrcColor: return vk::BlendFactor::eSrcColor;
		case BlendFactor::OneMinusSrcColor: return vk::BlendFactor::eOneMinusSrcColor;
		case BlendFactor::SrcAlpha: return vk::BlendFactor::eSrcAlpha;
		case BlendFactor::OneMinusSrcAlpha: return vk::BlendFactor::eOneMinusSrcAlpha;
		case BlendFactor::DstAlpha: return vk::BlendFactor::eDstAlpha;
		case BlendFactor::OneMinusDstAlpha: return vk::BlendFactor::eOneMinusDstAlpha;
		case BlendFactor::DstColor: return vk::BlendFactor::eDstColor;
		case BlendFactor::OneMinusDstColor: return vk::BlendFactor::eOneMinusDstColor;
		default:
			throw std::invalid_argument("Unsupported Topology value");
		}
	}
	static vk::BlendOp Convert(BlendOp value) {
		switch (value) {
		case BlendOp::Add: return vk::BlendOp::eAdd;
		case BlendOp::Sub: return vk::BlendOp::eSubtract;
		case BlendOp::RevSub: return vk::BlendOp::eReverseSubtract;
		case BlendOp::Min: return vk::BlendOp::eMin;
		case BlendOp::Max: return vk::BlendOp::eMax;
		default:
			throw std::invalid_argument("Unsupported Topology value");
		}
	}
	static vk::StencilOp Convert(StencilOp value) {
		switch (value) {
		case StencilOp::Keep: return vk::StencilOp::eKeep;
		case StencilOp::Replace: return vk::StencilOp::eReplace;
		case StencilOp::Zero: return vk::StencilOp::eZero;
		case StencilOp::Invert: return vk::StencilOp::eInvert;
		case StencilOp::IncrementAndClamp: return vk::StencilOp::eIncrementAndClamp;
		case StencilOp::DecrementAndClamp: return vk::StencilOp::eDecrementAndClamp;
		case StencilOp::IncrementAndWrap: return vk::StencilOp::eIncrementAndWrap;
		case StencilOp::DecrementAndWrap: return vk::StencilOp::eDecrementAndWrap;
		default:
			throw std::invalid_argument("Unsupported Topology value");
		}
	}
	static vk::ColorComponentFlags Convert(ColorMask value) {
		vk::ColorComponentFlags result{ 0 };
		if (value & ColorCompoent::R) result |= vk::ColorComponentFlagBits::eR;
		if (value & ColorCompoent::G) result |= vk::ColorComponentFlagBits::eG;
		if (value & ColorCompoent::B) result |= vk::ColorComponentFlagBits::eB;
		if (value & ColorCompoent::A) result |= vk::ColorComponentFlagBits::eA;
		return result;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineStateImpl::PipelineStateImpl(VulkanRHI& rhi,const PipelineStateDesc& desc)
		: m_desc(desc)
	{

		if (!desc.rootSignature) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。RootSignatureが設定されていません。");
			return;
		}
		
		if (!m_desc.vs) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。頂点シェーダが設定されていません。");
			return;
		}
		if (!m_desc.ps) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。ピクセルシェーダが設定されていません。");
			return;
		}

		auto& device = rhi.getDevice();

		FixedVector<vk::PipelineShaderStageCreateInfo, 8> stages;
        FixedVector<vk::VertexInputBindingDescription, 16> bindingDescriptions;
        FixedVector<vk::VertexInputAttributeDescription, 16> attributeDescriptions;

        

		if (auto shader = desc.vs.cast<ShaderImpl>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eVertex;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Vertex);
		}
		if (auto shader = desc.vs.cast<ShaderImpl>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eFragment;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Pixel);
		}
		// TODO 他ステージの対応

		uint32_t vertexLocation = 0;
		uint32_t instanceLocation = 0;

        for (auto& attribute : desc.vertexLayout.attributes) {
            auto& attr = attributeDescriptions.emplace_back();
			attr.binding = Convert(attribute.inputRate);
			attr.offset = attribute.offset;
			attr.format = Convert(attribute.type, attribute.dimention);

			if (attribute.inputRate == VertexInputRate::Vertex) {
				attr.location = vertexLocation++;
			}
			else if (attribute.inputRate == VertexInputRate::Instance) {
				attr.location = instanceLocation++;
			}
        }
		{
			auto& item = bindingDescriptions.emplace_back();
			item.binding = 0;
			item.stride = desc.vertexLayout.vertexStride;
			item.inputRate = vk::VertexInputRate::eVertex;
		}
		if(false){
			auto& item = bindingDescriptions.emplace_back();
			item.binding = 1;
			item.stride = desc.vertexLayout.instanceStride;
			item.inputRate = vk::VertexInputRate::eInstance;
		}


		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
        vertexInputInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        inputAssemblyInfo.topology = Convert(desc.topology);
        inputAssemblyInfo.primitiveRestartEnable = false;

		// vk::PipelineTessellationStateCreateInfo tessellationInfo;
		// tessellationInfo.patchControlPoints = 3;


		// vk::Viewport viewport;
		// vk::Rect2D scissor;
		// vk::PipelineViewportStateCreateInfo viewportInfo;
		// viewportInfo.viewportCount = 1;
		// viewportInfo.scissorCount = 1;
		// viewportInfo.pViewports = nullptr;
		// viewportInfo.pScissors = nullptr;


		vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
		rasterizationInfo.flags = vk::PipelineRasterizationStateCreateFlags{};
        rasterizationInfo.depthClampEnable = false; // TODO 確認
        rasterizationInfo.rasterizerDiscardEnable = true; // Maskedの時だけtrueにしたい
		rasterizationInfo.polygonMode = Convert(desc.rasterizer.fillMode);
        rasterizationInfo.cullMode = Convert(desc.rasterizer.cullMode);
        rasterizationInfo.frontFace = vk::FrontFace::eClockwise;
		rasterizationInfo.depthBiasEnable = false;
		rasterizationInfo.lineWidth = 1.0f;

		vk::PipelineMultisampleStateCreateInfo multisampleInfo;
        multisampleInfo.sampleShadingEnable = false;
        multisampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;

		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
        depthStencilInfo.depthTestEnable = desc.depthStencil.depth.enable;
        depthStencilInfo.depthWriteEnable = desc.depthStencil.depth.write;
		depthStencilInfo.depthCompareOp = Convert(desc.depthStencil.depth.func);
		depthStencilInfo.depthBoundsTestEnable = false;
		// depthStencilInfo.minDepthBounds = ;
		// depthStencilInfo.maxDepthBounds = ;
        depthStencilInfo.stencilTestEnable = desc.depthStencil.stencil.enable;

        depthStencilInfo.front.failOp = Convert(desc.depthStencil.stencil.failOp);
        depthStencilInfo.front.depthFailOp = Convert(desc.depthStencil.stencil.depthFailOp);
        depthStencilInfo.front.passOp = Convert(desc.depthStencil.stencil.passOp);
        depthStencilInfo.front.compareOp = vk::CompareOp::eAlways; // TODO 対応するか
        depthStencilInfo.front.compareMask = desc.depthStencil.stencil.reference;
        depthStencilInfo.front.writeMask = desc.depthStencil.stencil.write_mask;
        depthStencilInfo.front.reference = desc.depthStencil.stencil.read_mask;
		depthStencilInfo.back = depthStencilInfo.front;


		FixedVector<vk::PipelineColorBlendAttachmentState, 8> blendAttachments;
		for (auto& blend : desc.blend) {
			auto& item = blendAttachments.emplace_back();
			item.blendEnable = blend.blendEnable;
			item.srcColorBlendFactor = Convert(blend.srcColorFactor);
			item.dstColorBlendFactor = Convert(blend.dstColorFactor);
			item.colorBlendOp = Convert(blend.colorOp);
			item.srcAlphaBlendFactor = Convert(blend.srcAlphaFactor);
			item.dstAlphaBlendFactor = Convert(blend.dstAlphaFactor);
			item.alphaBlendOp = Convert(blend.alphaOp);
			item.colorWriteMask = Convert(blend.mask);
		}
		vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = vk::LogicOp::eCopy;
        colorBlendInfo.attachmentCount = static_cast<uint32_t>(desc.colors.size());
        colorBlendInfo.pAttachments = nullptr; // TODO: Provide blend attachment states
        colorBlendInfo.blendConstants[0] = 0.0f;
        colorBlendInfo.blendConstants[1] = 0.0f;
        colorBlendInfo.blendConstants[2] = 0.0f;
        colorBlendInfo.blendConstants[3] = 0.0f;


		FixedVector<vk::DynamicState, 8> dynamicStates{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor,
		};
		vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
		dynamicStateInfo.dynamicStateCount = dynamicStates.size();
		dynamicStateInfo.pDynamicStates = dynamicStates.data();
		dynamicStateInfo.flags = vk::PipelineDynamicStateCreateFlags{};


		vk::GraphicsPipelineCreateInfo info;
		info.flags = vk::PipelineCreateFlagBits{};
		info.stageCount = stages.size();
		info.pStages = stages.data();
		info.pVertexInputState = &vertexInputInfo;
		info.pInputAssemblyState = &inputAssemblyInfo;
		info.pTessellationState = nullptr;// &tessellationInfo;
		info.pViewportState = nullptr;// &viewportInfo;
		info.pRasterizationState = &rasterizationInfo;
		info.pMultisampleState = &multisampleInfo;
		info.pDepthStencilState = &depthStencilInfo;
		info.pColorBlendState = &colorBlendInfo;
		info.pDynamicState = &dynamicStateInfo;
		info.layout = desc.rootSignature.cast<RootSignatureImpl>()->getNative();
		info.renderPass;
		info.subpass;
		info.basePipelineHandle;
		info.basePipelineIndex;

		m_pipeline = device.createGraphicsPipeline(nullptr,info,rhi.getAllocationCallbacks());

		Utility::SetName(device, m_pipeline, m_desc.name);

	}

	const String& PipelineStateImpl::getName() const
	{
		return m_desc.name;
	}



}
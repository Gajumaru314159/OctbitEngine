//***********************************************************
//! @file
//! @brief		パイプラインステート実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/PipelineState/VulkanPipelineState.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Shader/VulkanShader.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>

namespace ob::rhi::vulkan {

	//! @brief		コンストラクタ
	VulkanPipelineState::VulkanPipelineState(VulkanRHI& rhi,const PipelineStateDesc& desc)
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

        

		if (auto shader = desc.vs.cast<VulkanShader>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eVertex;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Vertex);
		}
		if (auto shader = desc.ps.cast<VulkanShader>()) {
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
			attr.binding = TypeConverter::Convert(attribute.inputRate);
			attr.offset = attribute.offset;
			attr.format = TypeConverter::Convert(attribute.type, attribute.dimention);

			if (attribute.inputRate == VertexInputRate::Vertex) {
				attr.location = vertexLocation++;
				attr.offset = attr.offset;
			}
			else if (attribute.inputRate == VertexInputRate::Instance) {
				attr.location = instanceLocation++;
				attr.offset = attr.offset;
			}
        }
		{
			auto& item = bindingDescriptions.emplace_back();
			item.binding = 0;
			item.stride = desc.vertexLayout.vertexStride;
			item.inputRate = vk::VertexInputRate::eVertex;

			OB_ASSERT_EXPR(0 < item.stride);
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
        inputAssemblyInfo.topology = TypeConverter::Convert(desc.topology);
        inputAssemblyInfo.primitiveRestartEnable = false;

		vk::PipelineTessellationStateCreateInfo tessellationInfo;

		vk::Viewport viewport[8];
		vk::Rect2D scissor[8];
		vk::PipelineViewportStateCreateInfo viewportInfo;
		viewportInfo.viewportCount = desc.colors.size();
		viewportInfo.scissorCount = desc.colors.size();
		viewportInfo.pViewports = viewport;
		viewportInfo.pScissors = scissor;

		vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
		rasterizationInfo.flags = vk::PipelineRasterizationStateCreateFlags{};
        rasterizationInfo.depthClampEnable = false; // TODO 確認
        rasterizationInfo.rasterizerDiscardEnable = false;
		rasterizationInfo.polygonMode = TypeConverter::Convert(desc.rasterizer.fillMode);
        rasterizationInfo.cullMode = TypeConverter::Convert(desc.rasterizer.cullMode);
        rasterizationInfo.frontFace = vk::FrontFace::eClockwise;
		rasterizationInfo.depthBiasEnable = false;
		rasterizationInfo.lineWidth = 1.0f;

		vk::PipelineMultisampleStateCreateInfo multisampleInfo;
        multisampleInfo.sampleShadingEnable = false;
        multisampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;

		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
        depthStencilInfo.depthTestEnable = desc.depthStencil.depth.enable;
        depthStencilInfo.depthWriteEnable = desc.depthStencil.depth.write;
		depthStencilInfo.depthCompareOp = TypeConverter::Convert(desc.depthStencil.depth.func);
		depthStencilInfo.depthBoundsTestEnable = false;
		// depthStencilInfo.minDepthBounds = ;
		// depthStencilInfo.maxDepthBounds = ;
        depthStencilInfo.stencilTestEnable = desc.depthStencil.stencil.enable;

        depthStencilInfo.front.failOp = TypeConverter::Convert(desc.depthStencil.stencil.failOp);
        depthStencilInfo.front.depthFailOp = TypeConverter::Convert(desc.depthStencil.stencil.depthFailOp);
        depthStencilInfo.front.passOp = TypeConverter::Convert(desc.depthStencil.stencil.passOp);
        depthStencilInfo.front.compareOp = vk::CompareOp::eAlways; // TODO 対応するか
        depthStencilInfo.front.compareMask = desc.depthStencil.stencil.reference;
        depthStencilInfo.front.writeMask = desc.depthStencil.stencil.write_mask;
        depthStencilInfo.front.reference = desc.depthStencil.stencil.read_mask;
		depthStencilInfo.back = depthStencilInfo.front;


		FixedVector<vk::PipelineColorBlendAttachmentState, 8> blendAttachments;
		for (auto& blend : desc.blend) {
			auto& item = blendAttachments.emplace_back();
			item.blendEnable = blend.blendEnable;
			item.srcColorBlendFactor = TypeConverter::Convert(blend.srcColorFactor);
			item.dstColorBlendFactor = TypeConverter::Convert(blend.dstColorFactor);
			item.colorBlendOp = TypeConverter::Convert(blend.colorOp);
			item.srcAlphaBlendFactor = TypeConverter::Convert(blend.srcAlphaFactor);
			item.dstAlphaBlendFactor = TypeConverter::Convert(blend.dstAlphaFactor);
			item.alphaBlendOp = TypeConverter::Convert(blend.alphaOp);
			item.colorWriteMask = TypeConverter::Convert(blend.mask);
		}
		vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = vk::LogicOp::eCopy;
        colorBlendInfo.attachmentCount = static_cast<uint32_t>(desc.colors.size());
        colorBlendInfo.pAttachments = blendAttachments.data();
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
		info.pTessellationState = &tessellationInfo;
		info.pViewportState = &viewportInfo;
		info.pRasterizationState = &rasterizationInfo;
		info.pMultisampleState = &multisampleInfo;
		info.pDepthStencilState = &depthStencilInfo;
		info.pColorBlendState = &colorBlendInfo;
		info.pDynamicState = &dynamicStateInfo;
		info.layout = desc.rootSignature.cast<VulkanRootSignature>()->getNative();
		info.renderPass;
		info.subpass;
		info.basePipelineHandle;
		info.basePipelineIndex;

		vk::PipelineRenderingCreateInfo renderingInfo;
		FixedVector<vk::Format, 8> colorFormats;
		for (auto& color : desc.colors) {
			colorFormats.emplace_back(TypeConverter::Convert(color));
		}
		if (desc.depth) {
			renderingInfo.setDepthAttachmentFormat(TypeConverter::Convert(desc.depth.value()));
		}
		renderingInfo.setColorAttachmentFormats(colorFormats);
		info.pNext = &renderingInfo;

		m_pipeline = device.createGraphicsPipeline(nullptr,info,rhi.getAllocationCallbacks());

		rhi.setName(m_pipeline, m_desc.name);

		manage();
	}

	const String& VulkanPipelineState::getName() const
	{
		return m_desc.name;
	}

}
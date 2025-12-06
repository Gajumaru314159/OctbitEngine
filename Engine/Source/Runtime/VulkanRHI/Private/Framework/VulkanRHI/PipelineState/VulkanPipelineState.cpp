//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/VulkanRHI/PipelineState/VulkanPipelineState.h>
#include <Framework/VulkanRHI/VulkanDevice.h>
#include <Framework/VulkanRHI/Shader/VulkanShader.h>
#include <Framework/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/VulkanRHI/RootSignature/VulkanRootSignature.h>

namespace ob::rhi {

	//! @brief		コンストラクタ
	VulkanPipelineState::VulkanPipelineState(VulkanDevice& device,const PipelineStateDesc& desc)
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

		m_layout = desc.rootSignature.cast<VulkanRootSignature>()->getNative();

		auto& vkdevice = device.getDevice();

		FixedVector<vk::PipelineShaderStageCreateInfo, 8> stages;
        FixedVector<vk::VertexInputBindingDescription, 16> bindingDescriptions;
        FixedVector<vk::VertexInputAttributeDescription, 16> attributeDescriptions;

		// シェーダステージの設定
		if (auto shader = desc.vs.cast<VulkanShader>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eVertex;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Vertex);
		}
		if (auto shader = desc.gs.cast<VulkanShader>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eGeometry;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Geometry);
		}
		if (auto shader = desc.hs.cast<VulkanShader>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eTessellationControl;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Hull);
		}
		if (auto shader = desc.ds.cast<VulkanShader>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Domain);
		}
		if (auto shader = desc.ps.cast<VulkanShader>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eFragment;
			stage.module = shader->getNative();
			stage.pName = Shader::GetEntryName(ShaderStage::Pixel);
		}

		// 頂点レイアウトの設定
		uint32_t vertexLocation = 0;
		uint32_t instanceLocation = 0;

        for (auto& attribute : desc.vertexLayout.attributes) {
            auto& attr = attributeDescriptions.emplace_back();
			attr.binding = TypeConverter::Convert(attribute.inputRate);
			attr.offset = attribute.offset;
			attr.format = TypeConverter::Convert(attribute.type, attribute.dimension);

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
			OB_ASSERT_EXPR(0 < item.stride);
		}


		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        inputAssemblyInfo.topology = TypeConverter::Convert(desc.topology);
        inputAssemblyInfo.primitiveRestartEnable = false;

		vk::PipelineTessellationStateCreateInfo tessellationInfo;

		vk::Viewport viewport;
		vk::Rect2D scissor;
		vk::PipelineViewportStateCreateInfo viewportInfo;
		viewportInfo.viewportCount = 1;
		viewportInfo.scissorCount = 1;
		viewportInfo.pViewports = &viewport;
		viewportInfo.pScissors = &scissor;

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
		// RenderPassは動的設定する

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

		m_pipeline = vkdevice.createGraphicsPipeline(nullptr,info,device.getAllocationCallbacks());

		device.setName(m_pipeline, m_desc.name);

		manage();
	}

	const String& VulkanPipelineState::getName() const {
		return m_desc.name;
	}

}
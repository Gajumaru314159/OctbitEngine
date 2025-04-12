//***********************************************************
//! @file
//! @brief		パイプラインステート実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/PipelineState/PipelineStateImpl.h>
//#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/VulkanRHI/Shader/ShaderImpl.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>

namespace ob::rhi::vulkan {


	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineStateImpl::PipelineStateImpl(const PipelineStateDesc& desc)
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

		FixedVector<vk::PipelineShaderStageCreateInfo, 8> stages;

		if (auto shader = desc.vs.cast<ShaderImpl>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eVertex;
			stage.module = shader->getModule();
			stage.pName = Shader::GetEntryName(ShaderStage::Vertex);
		}
		if (auto shader = desc.vs.cast<ShaderImpl>()) {
			auto& stage = stages.emplace_back();
			stage.flags = vk::PipelineShaderStageCreateFlags{};
			stage.stage = vk::ShaderStageFlagBits::eFragment;
			stage.module = shader->getModule();
			stage.pName = Shader::GetEntryName(ShaderStage::Pixel);
		}
		// TODO 他ステージの対応

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;


		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;


		vk::PipelineTessellationStateCreateInfo tessellationInfo;


		vk::PipelineViewportStateCreateInfo viewportInfo;


		vk::PipelineRasterizationStateCreateInfo rasterizationInfo;


		vk::PipelineMultisampleStateCreateInfo multisampleInfo;


		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;


		vk::PipelineColorBlendStateCreateInfo colorBlendInfo;


		vk::PipelineDynamicStateCreateInfo dynamicStateInfo;




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
		info.layout = desc.rootSignature.cast<RootSignatureImpl>()->getNative();
		info.renderPass;
		info.subpass;
		info.basePipelineHandle;
		info.basePipelineIndex;
		//vkCreateGraphicsPipelines(,,1,)

	}

	const String& PipelineStateImpl::getName() const
	{
		return m_desc.name;
	}



}
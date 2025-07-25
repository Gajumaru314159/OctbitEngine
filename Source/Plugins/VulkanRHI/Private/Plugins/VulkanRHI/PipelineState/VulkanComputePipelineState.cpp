//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/PipelineState/VulkanComputePipelineState.h>
#include <Plugins/VulkanRHI/VulkanDevice.h>
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>
#include <Plugins/VulkanRHI/Shader/VulkanShader.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>

namespace ob::rhi {


	//! @brief		コンストラクタ
	VulkanComputePipelineState::VulkanComputePipelineState(VulkanDevice& device, const ComputePipelineStateDesc& desc)
		: m_desc(desc)
	{

		if (!desc.rootSignature) {
			throw Exception("コンピュートパイプラインステートの構築に失敗。RootSignatureが設定されていません。");
		}

		if (!m_desc.cs) {
			throw Exception("コンピュートパイプラインステートの構築に失敗。コンピュートシェーダが設定されていません。");
			return;
		}

		auto& vkdevice = device.getDevice();

		// コンピュートシェーダステージ
		vk::PipelineShaderStageCreateInfo shaderStageInfo;
		shaderStageInfo.flags = vk::PipelineShaderStageCreateFlags{};
		shaderStageInfo.stage = vk::ShaderStageFlagBits::eCompute;

		if (m_desc.cs) {
			auto& shader = *m_desc.cs.cast<VulkanShader>();
			shaderStageInfo.module = shader.getNative();
			shaderStageInfo.pName = Shader::GetEntryName(ShaderStage::Compute);
		}

		// コンピュートパイプラインの定義
		vk::ComputePipelineCreateInfo pipelineInfo;
		pipelineInfo.flags = vk::PipelineCreateFlags{};
		pipelineInfo.stage = shaderStageInfo;
		pipelineInfo.layout = desc.rootSignature.cast<VulkanRootSignature>()->getNative();

		// コンピュートパイプラインを生成
		m_pipeline = vkdevice.createComputePipeline(nullptr, pipelineInfo, device.getAllocationCallbacks());

		manage();
	}


	//! @brief		デストラクタ
	VulkanComputePipelineState::~VulkanComputePipelineState() {
		// vk::raii::Pipeline automatically destroys the pipeline
	}


	//! @brief      名前を取得
	const String& VulkanComputePipelineState::getName()const {
		return m_desc.name;
	}

}
//***********************************************************
//! @file
//! @brief		パイプラインステート実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>
//#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/VulkanRHI/Shader/ShaderImpl.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi::vulkan {


	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineStateImpl::PipelineStateImpl(DeviceImpl& rDevice, const PipelineStateDesc& desc)
		: m_desc(desc)
	{

		if (!desc.rootSignature) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。RootSignatureが設定されていません。");
			return;
		}
		if (!desc.target) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。RenderTargetが設定されていません。");
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

		const auto targetCount = desc.target.getColorTextureCount();
		if (8 < targetCount) {
			LOG_FATAL_EX("Graphic", "PipelineStateの構築に失敗。描画先枚数が8以上です。[num={}]", targetCount);
			return;
		}


		VkGraphicsPipelineCreateInfo info{};


		//vkCreateGraphicsPipelines(,,1,)

	}



}
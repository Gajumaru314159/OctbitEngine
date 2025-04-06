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


		VkGraphicsPipelineCreateInfo info{};


		//vkCreateGraphicsPipelines(,,1,)

	}

	const String& PipelineStateImpl::getName() const
	{
		return m_desc.name;
	}



}
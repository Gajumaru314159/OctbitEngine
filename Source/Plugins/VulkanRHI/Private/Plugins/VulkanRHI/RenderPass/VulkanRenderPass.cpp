//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RenderPass/VulkanRenderPass.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//! @brief				シェーダーコードからシェーダーオブジェクトを生成
	//!
	//! @param src			シェーダコード
	//! @param stage		シェーダステージ
	//! @param errorDest	エラー出力先文字列
	VulkanRenderPass::VulkanRenderPass(VulkanRHI& rhi, const RenderPassDesc& desc)
	{
		vk::RenderPassCreateInfo info;
		info.setAttachmentCount((s32)desc.attachments.size());
		Vector<vk::AttachmentDescription> attachments(desc.attachments.size());
		for (size_t i = 0; i < desc.attachments.size(); ++i) {
			auto& source = desc.attachments[i];
			auto& attachment = attachments[i];
			attachment.format = TypeConverter::Convert(source.format);
			attachment.samples = vk::SampleCountFlagBits::e1;
			attachment.loadOp = TypeConverter::Convert(source.load);
			attachment.storeOp = TypeConverter::Convert(source.store);
			attachment.stencilLoadOp = TypeConverter::Convert(source.load);
			attachment.stencilStoreOp = TypeConverter::Convert(source.store);
			attachment.initialLayout = TypeConverter::Convert(source.initialState);
			attachment.finalLayout = TypeConverter::Convert(source.finalState);
		}

		info.setAttachments(attachments);

		vk::SubpassDescription subpass;
		//subpass.setInputAttachments();
		//subpass.setColorAttachments();


		m_renderPass = rhi.getDevice().createRenderPass(info,rhi.getAllocationCallbacks());
	}

}
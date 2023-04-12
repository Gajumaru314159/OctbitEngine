//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RenderPass/RenderPassImpl.h>
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderPassImpl::RenderPassImpl(VkDevice device,const RenderPassDesc& desc)
		: m_desc(desc) 
		, m_device(device)
	{
		OB_CHECK_ASSERT_EXPR(device!=nullptr);

		Array<VkAttachmentDescription> attachments;
		Array<VkAttachmentReference> referencies;
		Array<uint32_t> preserves;
		Array<VkSubpassDescription> subpesses;
		Array<VkSubpassDependency> dependencies;

		// ポインタ変更がないように事前確保
		referencies.reserve(desc.attachments.size() * desc.subpasses.size());
		preserves.reserve(desc.attachments.size() * desc.subpasses.size());

		// VkAttachmentDescription
		for (auto& attachment : desc.attachments) {

			auto& a = attachments.emplace_back();
			a.format = TypeConverter::Convert(attachment.format);
			a.samples = VK_SAMPLE_COUNT_1_BIT;
			a.loadOp = TypeConverter::Convert(attachment.clear);
			a.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			a.stencilLoadOp = TypeConverter::Convert(attachment.clear);
			a.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
			a.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			a.finalLayout = TypeConverter::Convert(attachment.finalState);
			a.flags = 0;

		}

		// VkSubpassDescription
		for (auto& subpass : desc.subpasses) {

			Array<bool> unused(desc.attachments.size(),true);

			const auto addRefs = [&referencies,&unused](const Array<AttachmentReference>& refs)->VkAttachmentReference* {

				if (refs.empty())return nullptr;

				size_t front = referencies.size();
				for (auto& i : refs) {
					auto& ref = referencies.emplace_back();
					ref.attachment = i.index;
					ref.layout = TypeConverter::Convert(i.state);
					unused.at(ref.attachment) = false;
				}
				return &referencies[front];

			};

			auto pInputAttachments = addRefs(subpass.inputs);
			auto pColorAttachments = addRefs(subpass.colors);
			auto pDepthAttachment  = addRefs(subpass.depth);

			// preserve
			uint32_t preserveBegin = (uint32_t)preserves.size();
			uint32_t preserveCount = 0;
			for (auto [index,ref] : Indexed(desc.attachments)) {
				if (unused[index]) {
					preserves.push_back(index);
					preserveCount++;
				}
			}

			auto& s = subpesses.emplace_back();
			s.flags = 0;
			s.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			s.inputAttachmentCount = (uint32_t)subpass.inputs.size();
			s.pInputAttachments = pInputAttachments;
			s.colorAttachmentCount = (uint32_t)subpass.colors.size();
			s.pColorAttachments = pColorAttachments;
			s.pResolveAttachments = nullptr;
			s.pDepthStencilAttachment = pDepthAttachment;
			s.preserveAttachmentCount = preserveCount;
			s.pPreserveAttachments = preserveCount?&preserves[preserveBegin]:nullptr;

		}

		// VkSubpassDependency
		for (auto& dep : desc.dependencies) {
			auto& d= dependencies.emplace_back();
			d.srcSubpass = dep.srcSubpass == -1 ? VK_SUBPASS_EXTERNAL : (uint32_t)dep.srcSubpass;
			d.dstSubpass = dep.dstSubpass == -1 ? VK_SUBPASS_EXTERNAL : (uint32_t)dep.dstSubpass;
			d.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			d.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			d.srcAccessMask = 0;
			d.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			d.dependencyFlags = 0;
		}


		::VkRenderPassCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.pNext = nullptr;
		info.attachmentCount = (uint32_t)attachments.size();
		info.pAttachments = attachments.data();
		info.subpassCount = (uint32_t)subpesses.size();
		info.pSubpasses = subpesses.data();
		info.dependencyCount = (uint32_t)dependencies.size();
		info.pDependencies = dependencies.data();

		if (Failed(::vkCreateRenderPass(device, &info, nullptr, &m_renderPass))) {
			LOG_ERROR("vkCreateRenderPassに失敗");
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	RenderPassImpl::~RenderPassImpl() {
		if (m_renderPass) {
			::vkDestroyRenderPass(m_device, m_renderPass, nullptr);
			m_renderPass = nullptr;
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool RenderPassImpl::isValid()const {
		return m_renderPass != nullptr;
	}

}
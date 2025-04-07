//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Texture/TextureUploader.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan
{

	//! @brief  コンストラクタ
	TextureUploader::TextureUploader(VulkanRHI& rhi)
		: m_rhi(rhi)
	{
		m_frames.resize(4);
	}

	//! @brief  アップロード要素を追加
	void TextureUploader::add(const vk::raii::Image& dest, Span<Subresource> subresources) {

		if (dest == nullptr) {
			LOG_ERROR("[TextureUploader] destがnullです。");
			return;
		}

		auto requirements = dest.getMemoryRequirements();

		// アップロードリソースを生成

		vk::BufferCreateInfo info;
		info.size = requirements.size;
		info.usage = vk::BufferUsageFlagBits::eTransferSrc;

		auto sourceResource = createUploadResource(info);
		if (sourceResource == nullptr) {
			LOG_ERROR("[TextureUploader] コピーソースリソースの生成に失敗しました");
			return;
		}

	}

    //! @brief アップロード用の一時リソースを作成 
	vk::raii::DeviceMemory TextureUploader::createUploadResource(const vk::BufferCreateInfo& info) {

		auto& device = m_rhi.getDevice();


		return nullptr;
	}


	void TextureUploader::update(vk::raii::CommandBuffer& commandBuffer) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();


	}

}
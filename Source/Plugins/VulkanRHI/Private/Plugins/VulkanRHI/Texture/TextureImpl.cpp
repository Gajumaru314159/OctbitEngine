//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "TextureImpl.h"
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//! @brief バリデート
	static bool IsInvalid(const TextureDesc& desc) {

		// フォーマット
		if (desc.format == TextureFormat::Unknown) {
			LOG_ERROR("Textureの生成に失敗。TextureFormat::Unknownは指定できません。[name={}]", desc.name);
			return true;
		}

		// サイズ
		bool isValidSize = true;

		if (desc.type == TextureType::Texture1D) {
			isValidSize &= 0 < desc.size.width && 1 == desc.size.height && 1 == desc.size.depth;
		}
		if (desc.type == TextureType::Texture2D) {
			isValidSize &= 0 < desc.size.width && 0 < desc.size.height && 1 == desc.size.depth;
		}
		if (desc.type == TextureType::Texture3D) {
			isValidSize &= 0 < desc.size.width && 0 < desc.size.height && 0 < desc.size.depth;
		}
		if (desc.type == TextureType::Cube) {
			isValidSize &= 0 < desc.size.width && 0 < desc.size.height && 1 == desc.size.depth;
		}
		if (!isValidSize) {
			LOG_ERROR("Textureの生成に失敗。サイズが不正です。[size={}]", desc.size);
			return true;
		}

		// 配列
		if (desc.type == TextureType::Texture3D && 0 < desc.arrayNum) {
			LOG_ERROR("Texture3Dは配列に対応していません [name={}]", desc.name);
			return true;
		}

		return false;
	}

	static VkImageCreateInfo CreateCreateInfo(TextureType type,TextureFormat format,Size size, s32 mipLevel, s32 arrayNum,StringView name) {
		VkImageCreateInfo info = {};
		info.sType;
		info.flags;
		info.format = TypeConverter::Convert(format);
		info.extent = {(u32)size.width,(u32)size.height,(u32)size.depth};
		info.mipLevels = mipLevel;
		info.arrayLayers =arrayNum;
		info.samples = VK_SAMPLE_COUNT_1_BIT;
		info.tiling;
		info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		info.sharingMode;
		info.queueFamilyIndexCount;
		info.pQueueFamilyIndices;
		info.initialLayout;

		VK_IMAGE_TYPE_2D;


		switch (type) {
		case TextureType::Texture1D:
			info.imageType = VK_IMAGE_TYPE_1D;
			break;
		case TextureType::Texture2D:
			info.imageType = VK_IMAGE_TYPE_2D;
			break;
		case TextureType::Texture3D:
			info.imageType = VK_IMAGE_TYPE_3D;
			break;
		case TextureType::Cube:
			info.imageType = VK_IMAGE_TYPE_2D;
			break;
		default:
			LOG_ERROR("不明なテクスチャタイプです [name={}]", name);
			return {};
		}

	}


	//! @brief SizeからTextureTypeに変換 (Texture::Cube非対応) 
	static TextureType TextureTypeFrom(Size size) {
		if (size.height == 0) return TextureType::Texture1D;
		if (size.depth == 0) return TextureType::Texture2D;
		return TextureType::Texture3D;
	}


    //! @brief      TextureDesc から空のテクスチャを生成
    TextureImpl::TextureImpl(VkDevice device,const TextureDesc& desc)
		: m_desc(desc)
	{
		// バリデート
		if (IsInvalid(m_desc)) return;

		// 定義生成
		VkImageCreateInfo info = CreateCreateInfo(m_desc.type,m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		// ::VkResultvkCreateImage(device)


		if(Failed(vkCreateImage(device,&info,nullptr, &m_image))) return;

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(device, m_image, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = vkGetMemoryType get_device().get_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		VK_CHECK(vkAllocateMemory(get_device().get_handle(), &memory_allocate_info, nullptr, &mappable_memory));
		VK_CHECK(vkBindImageMemory(get_device().get_handle(), mappable_image, mappable_memory, 0));

    }


	//! @brief      IntColorの配列 から空のテクスチャを生成
	TextureImpl::TextureImpl(VkDevice device, StringView name, TextureType type,Size size, Span<const IntColor> colors)
	{
		// Desc設定
		m_desc.name = name;
		m_desc.size = size;
		m_desc.type = type;
		m_desc.format = TextureFormat::RGBA8;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (IsInvalid(m_desc)) return;

		if (std::max(size.width, 1) * std::max(size.height, 1) * std::max(size.depth, 1) != colors.size()) {
			LOG_ERROR("Textureの生成に失敗。サイズとcolors.size()が一致していません。[size={}, name={}]", size, name);
			return;
		}

	}


	//! @brief      テクスチャバイナリから生成
	TextureImpl::TextureImpl(VkDevice device, StringView name,BlobView blob)
	{
	}


	//! @brief       RenderTextureDesc からRenderTextureを生成
	TextureImpl::TextureImpl(VkDevice device, const RenderTextureDesc& desc)
		: m_renderDesc(desc)
	{
		m_desc.name = desc.name;
		m_desc.size = desc.size;
		m_desc.type = TextureType::Texture2D;
		m_desc.format = desc.format;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (IsInvalid(m_desc)) return;

	}


	//! @brief      SwapChainのリソースからRenderTextureを生成
	TextureImpl::TextureImpl(VkDevice Device, VkImage view,StringView name) {

	}

}// ob::rhi::dx12
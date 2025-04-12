//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Texture/TextureImpl.h>
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

	static vk::ImageCreateInfo CreateCreateInfo(TextureType type,TextureFormat format,Size size, s32 mipLevel, s32 arrayNum,StringView name) {
		VkImageCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.flags = 0;
		info.format = TypeConverter::Convert(format);
		info.extent = {(u32)size.width,(u32)size.height,(u32)size.depth};
		info.mipLevels = std::max(mipLevel,1);
		info.arrayLayers =arrayNum;
		info.samples = VK_SAMPLE_COUNT_1_BIT;
		info.tiling = VK_IMAGE_TILING_LINEAR; // VK_IMAGE_TILING_OPTIMAL; 直接アップロード用の仮対応
		info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = nullptr;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

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

		return info;
	}


	//! @brief SizeからTextureTypeに変換 (Texture::Cube非対応) 
	static TextureType TextureTypeFrom(Size size) {
		if (size.height == 0) return TextureType::Texture1D;
		if (size.depth == 0) return TextureType::Texture2D;
		return TextureType::Texture3D;
	}


    //! @brief      TextureDesc から空のテクスチャを生成
    TextureImpl::TextureImpl(VulkanRHI& rhi,const TextureDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{
		// バリデート
		if (IsInvalid(m_desc)) return;

		auto& device = m_rhi.getDevice();

		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type,m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		m_image = device.createImage(info, m_rhi.getAllocationCallbacks());

		auto requirements = m_image.getMemoryRequirements();


		auto allocInfo = rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		m_image.bindMemory(m_memory, 0);

    }


	//! @brief      IntColorの配列 から空のテクスチャを生成
	TextureImpl::TextureImpl(VulkanRHI& rhi, StringView name, TextureType type,Size size, Span<const IntColor> colors)
		: m_rhi(rhi)
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

		auto& device = m_rhi.getDevice();


		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type, m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		m_image = device.createImage(info, m_rhi.getAllocationCallbacks());

		auto requirements = m_image.getMemoryRequirements();


		auto allocInfo = rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		m_image.bindMemory(m_memory, 0);


		void* data = m_memory.mapMemory(0,requirements.size);
		memcpy_s(data, requirements.size, colors.data(), colors.size_bytes());
		m_memory.unmapMemory();

		// TODO 
		// コピー
		// バリア

	}


	//! @brief      テクスチャバイナリから生成
	TextureImpl::TextureImpl(VulkanRHI& rhi, StringView name,BlobView blob)
		: m_rhi(rhi)
	{
		//ファイルパスからVkImageを生成する

	}


	//! @brief       RenderTextureDesc からRenderTextureを生成
	TextureImpl::TextureImpl(VulkanRHI& rhi, const RenderTextureDesc& desc)
		: m_rhi(rhi)
		, m_renderDesc(desc)
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
	TextureImpl::TextureImpl(VulkanRHI& rhi, VkImage view,StringView name)
		: m_rhi(rhi)
	{

	}


	TextureImpl::~TextureImpl() {
	}

}// ob::rhi::dx12
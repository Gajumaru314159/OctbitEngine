//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
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

    vk::Format Convert(TextureFormat value) {
		switch (value) {
		case TextureFormat::RGBA32:         return vk::Format::eR32G32B32A32Sfloat;
		case TextureFormat::RGBA16:         return vk::Format::eR16G16B16A16Sfloat;
		case TextureFormat::RGBA8:          return vk::Format::eR8G8B8A8Unorm;

		case TextureFormat::RGBA8_SRGB:     return vk::Format::eR8G8B8A8Srgb;


		// case TextureFormat::RGB32:          return vk::Format::eR32G32B32Sfloat;
		// case TextureFormat::RGB8:           return vk::Format::eR8G8B8Unorm;

		case TextureFormat::RG32:           return vk::Format::eR32G32Sfloat;
		case TextureFormat::RG16:           return vk::Format::eR16G16Sfloat;
		case TextureFormat::RG8:            return vk::Format::eR8G8Unorm;

		case TextureFormat::R32:            return vk::Format::eR32Sfloat;
		case TextureFormat::R16:            return vk::Format::eR16Sfloat;
		case TextureFormat::R8:             return vk::Format::eR8Unorm;

		case TextureFormat::R10G10B10A2:    return vk::Format::eA2R10G10B10UnormPack32;

		case TextureFormat::D32S8:          return vk::Format::eD32SfloatS8Uint;
		case TextureFormat::D32:            return vk::Format::eD32Sfloat;
		case TextureFormat::D24S8:          return vk::Format::eD24UnormS8Uint;
		case TextureFormat::D16:            return vk::Format::eD16Unorm;

		case TextureFormat::BC1:            return vk::Format::eBc1RgbaUnormBlock;
		case TextureFormat::BC2:            return vk::Format::eBc2UnormBlock;
		case TextureFormat::BC3:            return vk::Format::eBc3UnormBlock;
		case TextureFormat::BC4:            return vk::Format::eBc4UnormBlock;
		case TextureFormat::BC5:            return vk::Format::eBc5UnormBlock;
		case TextureFormat::BC6H:           return vk::Format::eBc6HSfloatBlock;
		case TextureFormat::BC7:            return vk::Format::eBc7UnormBlock;

		case TextureFormat::BC1_SRGB:       return vk::Format::eBc1RgbaSrgbBlock;
		case TextureFormat::BC2_SRGB:       return vk::Format::eBc2SrgbBlock;
		case TextureFormat::BC3_SRGB:       return vk::Format::eBc3SrgbBlock;
		case TextureFormat::BC7_SRGB:       return vk::Format::eBc7SrgbBlock;
		}

		LOG_WARNING_EX("Graphic", "不正なTextureFormat[value={}]", enum_cast(value));
		return vk::Format::eUndefined;
    }

	static vk::ImageCreateInfo CreateCreateInfo(TextureType type,TextureFormat format,Size size, s32 mipLevel, s32 arrayNum,StringView name) {
		vk::ImageCreateInfo info;
		info.flags = {};
		info.format = Convert(format);
		info.extent = vk::Extent3D{(u32)size.width,(u32)size.height,(u32)size.depth};
		info.mipLevels = std::max(mipLevel,1);
		info.arrayLayers =std::max(arrayNum,1);
		info.samples = vk::SampleCountFlagBits::e1;
		info.tiling = vk::ImageTiling::eOptimal;
		info.usage = vk::ImageUsageFlags{} | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
		info.sharingMode = vk::SharingMode::eExclusive;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = nullptr;
		info.initialLayout = vk::ImageLayout::eUndefined;

		switch (type) {
		case TextureType::Texture1D:
			info.imageType = vk::ImageType::e1D;
			break;
		case TextureType::Texture2D:
			info.imageType = vk::ImageType::e2D;
			break;
		case TextureType::Texture3D:
			info.imageType = vk::ImageType::e3D;
			break;
		case TextureType::Cube:
			info.imageType = vk::ImageType::e2D;
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
    VulkanTexture::VulkanTexture(VulkanRHI& rhi,const TextureDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{
		// バリデート
		if (IsInvalid(m_desc)) throw Exception("Invalid TextureDesc");

		auto& device = m_rhi.getDevice();

		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type,m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		if (info.format == vk::Format::eUndefined) {
			LOG_ERROR("不正なフォーマットです [name={}]", m_desc.name);
			throw Exception("Invalid TextureDesc");
		}

		// リソース生成
		m_image = device.createImage(info, m_rhi.getAllocationCallbacks());

		auto requirements = m_image.getMemoryRequirements();


		auto allocInfo = rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		m_image.bindMemory(m_memory, 0);

    }


	//! @brief      IntColorの配列 から空のテクスチャを生成
	VulkanTexture::VulkanTexture(VulkanRHI& rhi, StringView name, TextureType type,Size size, Span<const IntColor> colors)
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
		if (IsInvalid(m_desc)) throw Exception("Invalid TextureDesc");

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


		auto allocInfo = rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		m_image.bindMemory(m_memory, 0);

		TextureUploader::Subresource subresources[1];
		subresources[0].data = BlobView(colors.data(),colors.size_bytes());

		m_rhi.getTextureUploader().add(m_image,info,subresources);

	}


	//! @brief      テクスチャバイナリから生成
	VulkanTexture::VulkanTexture(VulkanRHI& rhi, StringView name,BlobView blob)
		: m_rhi(rhi)
	{
		//ファイルパスからVkImageを生成する

	}


	//! @brief       RenderTextureDesc からRenderTextureを生成
	VulkanTexture::VulkanTexture(VulkanRHI& rhi, const RenderTextureDesc& desc)
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
		if (IsInvalid(m_desc)) throw Exception("Invalid TextureDesc");

		if(TextureFormatUtility::IsBC(m_desc.format) || m_desc.format == TextureFormat::RGB32 || m_desc.format == TextureFormat::RGB8 || m_desc.format == TextureFormat::Unknown) {
			// 上記2つのフォーマットだけvk::Errorではなくゼロ除算の構造化例外がcreateImageで発生するため個別対処
			throw Exception("Unsupported Foramt");
		}

		auto& device = m_rhi.getDevice();

		const bool isColor = !TextureFormatUtility::HasDepth(m_renderDesc.format);
		const bool isDepth = !isColor;


		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type, m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		if (isColor) {
			info.usage |= vk::ImageUsageFlagBits::eColorAttachment;
		}
		if (isDepth) {
			info.usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
		}

		auto info2 = (VkImageCreateInfo)info;

		// リソース生成
		m_image = device.createImage(info, m_rhi.getAllocationCallbacks());

		auto requirements = m_image.getMemoryRequirements();

		auto allocInfo = rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		m_image.bindMemory(m_memory, 0);


		// View
		vk::ImageViewCreateInfo viewCreateInfo;
		viewCreateInfo.viewType = vk::ImageViewType::e2D;
		viewCreateInfo.format = info.format;
		viewCreateInfo.components = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.subresourceRange.layerCount = 1;
		viewCreateInfo.image = m_image;

		if (isColor) {
			viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			m_hRTV = device.createImageView(viewCreateInfo);
		}
		if (isDepth) {
			viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			m_hDSV = device.createImageView(viewCreateInfo);
		}

	}


	//! @brief      SwapChainのリソースからRenderTextureを生成
	VulkanTexture::VulkanTexture(VulkanRHI& rhi, VkImage image, vk::Format format,StringView name)
		: m_rhi(rhi)
	{
		auto& device = m_rhi.getDevice();

		// View
		vk::ImageViewCreateInfo viewCreateInfo;
		viewCreateInfo.viewType = vk::ImageViewType::e2D;
		viewCreateInfo.format = format;
		viewCreateInfo.components = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.subresourceRange.layerCount = 1;
		viewCreateInfo.image = image;
		viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		m_hRTV = device.createImageView(viewCreateInfo);

	}


	VulkanTexture::~VulkanTexture() {
	}

}// ob::rhi::dx12
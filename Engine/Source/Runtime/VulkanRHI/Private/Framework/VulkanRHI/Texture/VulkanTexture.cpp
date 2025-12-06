//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/VulkanRHI/Texture/VulkanTexture.h>
#include <Framework/VulkanRHI/VulkanDevice.h>
#include <Framework/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/Core/String/FixedString.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ob::rhi {

	static vk::ImageCreateInfo CreateCreateInfo(TextureType type,TextureFormat format,Size size, s32 mipLevel, s32 arrayNum,StringView name) {
		vk::ImageCreateInfo info;
		info.flags = {};
		info.format = TypeConverter::Convert(format);
		info.extent = vk::Extent3D{static_cast<u32>(size.width),static_cast<u32>(size.height),static_cast<u32>(size.depth)};
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


    //! @brief      TextureDesc から空のテクスチャを生成
    VulkanTexture::VulkanTexture(VulkanDevice& device,const TextureDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		// バリデート
		if (!m_desc.isValid()) throw Exception("Invalid TextureDesc");

		auto& vkdevice = m_device.getDevice();

		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type,m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		m_shared = std::make_shared<SharedResource>();
		m_shared->image = vkdevice.createImage(info, m_device.getAllocationCallbacks());
		auto requirements = m_shared->image.getMemoryRequirements();
		auto allocInfo = device.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_shared->memory = vkdevice.allocateMemory(allocInfo, m_device.getAllocationCallbacks());
		m_shared->image.bindMemory(*m_shared->memory, 0);

		// 共通初期化
		initialize();
    }


	//! @brief      IntColorの配列 から空のテクスチャを生成
	VulkanTexture::VulkanTexture(VulkanDevice& device, StringView name, TextureType type,Size size, Span<const IntColor> colors)
		: m_device(device)
	{
		// Desc設定
		m_desc.name = name;
		m_desc.size = size;
		m_desc.type = type;
		m_desc.format = TextureFormat::RGBA8;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (!m_desc.isValid()) throw Exception("Invalid TextureDesc");

		if (std::max(size.width, 1) * std::max(size.height, 1) * std::max(size.depth, 1) != colors.size()) {
			LOG_ERROR("Textureの生成に失敗。サイズとcolors.size()が一致していません。[size={}, name={}]", size, name);
			return;
		}

		auto& vkdevice = m_device.getDevice();


		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type, m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		m_shared = std::make_shared<SharedResource>();
		m_shared->image = vkdevice.createImage(info, m_device.getAllocationCallbacks());
		auto requirements = m_shared->image.getMemoryRequirements();
		auto allocInfo = device.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_shared->memory = vkdevice.allocateMemory(allocInfo, m_device.getAllocationCallbacks());
		m_shared->image.bindMemory(*m_shared->memory, 0);

		// 色データをアップロード
		VulkanTextureUploader::Subresource subresources[1];
		subresources[0].data = BlobView(colors.data(),colors.size_bytes());
		m_device.getTextureUploader().add(m_shared->image, info, m_desc.format, subresources);

		// 共通初期化
		initialize();
	}


	//! @brief      テクスチャバイナリから生成
	VulkanTexture::VulkanTexture(VulkanDevice& device, StringView name,BlobView blob)
		: m_device(device)
	{

		// 定義生成
		FixedString<256> fname = name;
		int width, height, bpp;
		auto pixels = stbi_load(fname.data(), &width, &height, &bpp, 4);

		if (pixels == nullptr) throw Exception(Format("{}のピクセルデータの取得に失敗しました",name));

		auto f = gsl::final_action([&] {stbi_image_free(pixels); });

		// Desc設定
		m_desc.name = name;
		m_desc.size.width = width;
		m_desc.size.height = height;
		m_desc.type = TextureType::Texture2D;
		if (bpp == 1) m_desc.format = TextureFormat::R8;
		if (bpp == 2) m_desc.format = TextureFormat::RG8;
		if (bpp == 3) m_desc.format = TextureFormat::RGB8;
		if (bpp == 4) m_desc.format = TextureFormat::RGBA8;
		if (m_desc.format == TextureFormat::Unknown) throw Exception(Format("bpp={}はサポートされていません [file={}]", bpp,name));
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (!m_desc.isValid()) throw Exception("Invalid TextureDesc");

		auto& vkdevice = m_device.getDevice();

		// 定義生成
		vk::ImageCreateInfo info = CreateCreateInfo(m_desc.type, m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		m_shared = std::make_shared<SharedResource>();
		m_shared->image = vkdevice.createImage(info, m_device.getAllocationCallbacks());
		auto requirements = m_shared->image.getMemoryRequirements();
		auto allocInfo = device.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_shared->memory = vkdevice.allocateMemory(allocInfo, m_device.getAllocationCallbacks());
		m_shared->image.bindMemory(*m_shared->memory, 0);

		// 色データをアップロード
		VulkanTextureUploader::Subresource subresources[1];
		subresources[0].data = BlobView(pixels, width * height * bpp);
		m_device.getTextureUploader().add(m_shared->image, info, m_desc.format, subresources);

		// 共通初期化
		initialize();
	}


	//! @brief      ベースのテクスチャを指定して異なるビューを持つテクスチャを作成
	VulkanTexture::VulkanTexture(VulkanDevice& device, const TextureViewDesc& desc)
		: m_device(device)
	{
		auto base = desc.base.cast<VulkanTexture>();
		if (!base) {
			LOG_ERROR("ベーステクスチャが指定されていません");
			return;
		}

		m_desc = base->m_desc;
		m_viewDesc = desc;
		m_shared = base->m_shared;

		vk::DescriptorImageInfo info({}, *m_view, vk::ImageLayout::eShaderReadOnlyOptimal);
		vk::WriteDescriptorSet ddesc;
		ddesc.setImageInfo(info);

		switch (desc.type) {
		case TextureViewType::Texture:
			if (m_desc.flags & TextureFlag::ShaderResource) {
				createView(m_view);
				ddesc.descriptorType = vk::DescriptorType::eSampledImage;
			} else {
				throw Exception("TextureFlag::ShaderResourceが指定されていないTextureをTextureViewType::Textureで使用しようとしました");
			}
			break;
		case TextureViewType::RWTexture:
			if (m_desc.flags & TextureFlag::UnorderedAccess) {
				// createUAV(m_view,0);
				ddesc.descriptorType = vk::DescriptorType::eStorageImage;
			} else {
				throw Exception("TextureFlag::UnorderedAccessが指定されていないTextureをTextureViewType::RWTextureで使用しようとしました");
			}
			break;
		}

		m_device.allocateHandle(m_handle, ddesc);

		initialize();

	}



	//! @brief       RenderTextureDesc からRenderTextureを生成
	VulkanTexture::VulkanTexture(VulkanDevice& device, const RenderTextureDesc& desc)
		: m_device(device)
		, m_renderDesc(desc)
	{
		m_desc.name = desc.name;
		m_desc.size = desc.size;
		m_desc.type = TextureType::Texture2D;
		m_desc.format = desc.format;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (!m_renderDesc.isValid()) throw Exception("Invalid TextureDesc");
		if (!m_desc.isValid()) throw Exception("Invalid TextureDesc");

		auto& vkdevice = m_device.getDevice();

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

		// リソース生成
		m_shared = std::make_shared<SharedResource>();
		m_shared->image = vkdevice.createImage(info, m_device.getAllocationCallbacks());
		auto requirements = m_shared->image.getMemoryRequirements();
		auto allocInfo = device.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_shared->memory = vkdevice.allocateMemory(allocInfo, m_device.getAllocationCallbacks());
		m_shared->image.bindMemory(*m_shared->memory, 0);


		// ビュー生成
		vk::ImageViewCreateInfo viewCreateInfo;
		viewCreateInfo.viewType = vk::ImageViewType::e2D;
		viewCreateInfo.format = info.format;
		viewCreateInfo.components = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.subresourceRange.layerCount = 1;
		viewCreateInfo.image = *m_shared->image;

		// TODO DirectX12と異なりRTVとDSVはImageViewで管理できるので必要なもののみ生成する
		if (isColor) {
			viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			m_hRTV = vkdevice.createImageView(viewCreateInfo);
			device.setName(m_hRTV, m_desc.name);
		}
		if (isDepth) {
			viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			m_hDSV = vkdevice.createImageView(viewCreateInfo);
			device.setName(m_hDSV, m_desc.name);
		}

		device.setName(m_shared->image, m_desc.name);
		device.setName(m_shared->memory, m_desc.name);

		initialize();
	}


	//! @brief      SwapChainのリソースからRenderTextureを生成
	VulkanTexture::VulkanTexture(VulkanDevice& device, VkImage image, vk::Format format,vk::Extent2D size, StringView name)
		: m_device(device)
	{
		auto& vkdevice = m_device.getDevice();

		m_desc.name = name;
		m_desc.size = Size(size.width, size.height);
		m_desc.type = TextureType::Texture2D;
		m_desc.format = TypeConverter::Convert(format);
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// View
		vk::ImageViewCreateInfo viewCreateInfo;
		viewCreateInfo.viewType = vk::ImageViewType::e2D;
		viewCreateInfo.format = format;
		viewCreateInfo.components = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.subresourceRange.layerCount = 1;
		viewCreateInfo.image = image;
		viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		m_hRTV = vkdevice.createImageView(viewCreateInfo);

		initialize();
	}


	//! @brief 共通初期化処理
	//! @details テクスチャリソースはデフォルトでShaderResourceとして使用できるViewを持つ
	void VulkanTexture::initialize() {

		// View生成
		createView(m_view);

		// デスクリプタハンドルを割り当て
		vk::DescriptorImageInfo info({},*m_view,vk::ImageLayout::eShaderReadOnlyOptimal);
		vk::WriteDescriptorSet desc;
		desc.descriptorType = vk::DescriptorType::eSampledImage;
		desc.setImageInfo(info);

		m_device.allocateHandle(m_handle, desc);


		m_device.setName(m_shared->image, m_desc.name);
		m_device.setName(m_shared->memory, m_desc.name);

		manage();
	}


	VulkanTexture::~VulkanTexture() {
	}

	//! @brief TextureTypeからvk::ImageViewTypeに変換
	vk::ImageViewType Convert(TextureType type,s32 arrayNum) {
		switch (type) {
		case TextureType::Texture1D:	return 0  < arrayNum ? vk::ImageViewType::e1DArray : vk::ImageViewType::e1D;
		case TextureType::Texture2D:	return 0  < arrayNum ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D;
		case TextureType::Cube:			return 0 < arrayNum ? vk::ImageViewType::eCubeArray : vk::ImageViewType::eCube;
		case TextureType::Texture3D:	return vk::ImageViewType::e3D;
		}
		throw NotSupportedException();
	}

	bool VulkanTexture::createView(vk::raii::ImageView& view) const {

		vk::ImageViewCreateInfo info;
		info.image = *m_shared->image;
		info.viewType = Convert(m_desc.type, m_desc.arrayNum);
		info.format = TypeConverter::Convert(m_desc.format);
		info.components = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };
		info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = m_desc.mipLevels * std::max(m_desc.arrayNum,1);
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;

		if (m_desc.type == TextureType::Cube) info.subresourceRange.levelCount *= 6;

		// Depthの場合はAspectMaskを変更
		if (TextureFormatUtility::HasDepth(m_desc.format)) info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

		view = m_device.getDevice().createImageView(info);

		return true;
	}

}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Sampler/VulkanSampler.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi {

	//! @brief コンストラクタ 
	VulkanSampler::VulkanSampler(VulkanRHI& rhi, const SamplerDesc& desc)
	{
		m_name = desc.name;
		vk::SamplerCreateInfo info;
		info.flags = {};
		info.magFilter = TypeConverter::Convert(desc.filter); // 拡大時
		info.minFilter = TypeConverter::Convert(desc.filter);	// 縮小時
		info.mipmapMode = TypeConverter::Convert(desc.mipFilter);
		info.addressModeU = TypeConverter::Convert(desc.addressU);
		info.addressModeV = TypeConverter::Convert(desc.addressV);
		info.addressModeW = TypeConverter::Convert(desc.addressW);
		info.mipLodBias = desc.mipLodBias;
		info.anisotropyEnable = desc.anisotropy != Anisotropy::None;
		info.maxAnisotropy = TypeConverter::Convert(desc.anisotropy);
		info.compareEnable = false;
		info.compareOp = vk::CompareOp::eAlways;
		info.minLod = 0.0f;
		info.maxLod = 0.0f;
		info.borderColor = vk::BorderColor::eFloatOpaqueBlack;
		info.unnormalizedCoordinates = false;

		if (rhi.getFeatures().samplerAnisotropy == false && info.anisotropyEnable) {
			info.anisotropyEnable = false;
			info.maxAnisotropy = 0.0f;
		}

		auto& device = rhi.getDevice();
		m_sampler = device.createSampler(info,rhi.getAllocationCallbacks());

		// デスクリプタハンドルを割り当て
		vk::DescriptorImageInfo imageInfo(m_sampler);
		vk::WriteDescriptorSet writeDescSet;
		writeDescSet.descriptorType = vk::DescriptorType::eSampler;
		writeDescSet.setImageInfo(imageInfo);

		rhi.allocateHandle(m_handle, writeDescSet);


		rhi.setName(m_sampler, "Octbit Sampler");

		manage();
	}

}
//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Sampler/VulkanSampler.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {


	//! @brief				シェーダーコードからシェーダーオブジェクトを生成
	//!
	//! @param src			シェーダコード
	//! @param stage		シェーダステージ
	//! @param errorDest	エラー出力先文字列
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

		rhi.setName(m_sampler, "Octbit Sampler");

		manage();
	}

}
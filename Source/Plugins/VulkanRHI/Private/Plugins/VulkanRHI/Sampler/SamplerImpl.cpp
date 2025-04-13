//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Sampler/SamplerImpl.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan {

	static vk::Filter Convert(TextureFillter filter) {
		switch (filter) {
		case TextureFillter::Point: return vk::Filter::eNearest;
		case TextureFillter::Linear: return vk::Filter::eLinear;
		default: return vk::Filter::eLinear;
		}
	}

	static vk::SamplerMipmapMode Convert(MipFillter mipFilter) {
		switch (mipFilter) {
		case MipFillter::Point: return vk::SamplerMipmapMode::eNearest;
		case MipFillter::Linear: return vk::SamplerMipmapMode::eLinear;
		default: return vk::SamplerMipmapMode::eLinear;
		}
	}

	static vk::SamplerAddressMode Convert(TextureAddress address) {
		switch (address) {
		case TextureAddress::Repeat: return vk::SamplerAddressMode::eRepeat;
		case TextureAddress::Clamp: return vk::SamplerAddressMode::eClampToEdge;
		case TextureAddress::Mirror: return vk::SamplerAddressMode::eMirroredRepeat;
		default: return vk::SamplerAddressMode::eRepeat;
		}
	}

	static f32 Convert(Anisotropy anisotropy) {
		switch (anisotropy) {
		case Anisotropy::None: return 0;
		case Anisotropy::Level1: return 1;
		case Anisotropy::Level2: return 2;
		case Anisotropy::Level4: return 4;
		case Anisotropy::Level8: return 8;
		case Anisotropy::Level16: return 16;
		default: return 0;
		}
	}


	//! @brief				シェーダーコードからシェーダーオブジェクトを生成
	//!
	//! @param src			シェーダコード
	//! @param stage		シェーダステージ
	//! @param errorDest	エラー出力先文字列
	SamplerImpl::SamplerImpl(VulkanRHI& rhi, const SamplerDesc& desc)
	{
		m_name = desc.name;
		vk::SamplerCreateInfo info;
		info.flags = {};
		info.magFilter = Convert(desc.filter); // 拡大時
		info.minFilter = Convert(desc.filter);	// 縮小時
		info.mipmapMode = Convert(desc.mipFilter);
		info.addressModeU = Convert(desc.addressU);
		info.addressModeV = Convert(desc.addressV);
		info.addressModeW = Convert(desc.addressW);
		info.mipLodBias = desc.mipLodBias;
		info.anisotropyEnable = desc.anisotropy != Anisotropy::None;
		info.maxAnisotropy = Convert(desc.anisotropy);
		info.compareEnable = false;
		info.compareOp = vk::CompareOp::eAlways;
		info.minLod = 0.0f;
		info.maxLod = 0.0f;
		info.borderColor = vk::BorderColor::eFloatOpaqueBlack;
		info.unnormalizedCoordinates = false;

		auto& device = rhi.getDevice();
		m_sampler = device.createSampler(info,rhi.getAllocationCallbacks());

	}

}
//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>

namespace ob::rhi::vulkan {

	//! @brief  コンストラクタ
	RootSignatureImpl::RootSignatureImpl(const RootSignatureDesc& desc)
		: m_desc(desc)
	{
	
	}


	//! @brief  デストラクタ
	RootSignatureImpl::~RootSignatureImpl() {

	}


	//! @brief  妥当なオブジェクトか
	bool RootSignatureImpl::isValid()const {
		return false;
	}

	
	//! @brief      名前を取得
	const String& RootSignatureImpl::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const RootSignatureDesc& RootSignatureImpl::getDesc()const noexcept {
		return m_desc;
	}

}
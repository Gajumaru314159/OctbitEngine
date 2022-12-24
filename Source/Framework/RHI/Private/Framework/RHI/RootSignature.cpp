﻿//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/IRootSignature.h>

namespace ob::rhi {

	OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(RootSignature);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のRootSignatureへアクセス")

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RootSignature::RootSignature() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RootSignature::RootSignature(const RootSignatureDesc& desc) {
		m_pImpl = Device::Get()->createRootSignature(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		if (!m_pImpl->isValid()) {
			LOG_FATAL_EX("Graphic", "RootSignatureの生成に失敗");
			release();
		}
	}

}// namespace ob
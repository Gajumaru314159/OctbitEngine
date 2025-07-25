//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

	//! @brief  コンピュートパイプラインステート定義
	struct ComputePipelineStateDesc {
		
		String					name;							//!< 名前
		Ref<RootSignature>		rootSignature;					//!< ルートシグネチャ
		Ref<Shader>				cs;								//!< コンピュートシェーダ
	};

}
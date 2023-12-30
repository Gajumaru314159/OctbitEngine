//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

#include <Framework/RHI/CommandList.h>

namespace ob::rhi {

	class ShaderResourceGroupData {
		void setTexture();
		void setBuffer();
		void setSampler();
		void setConstants();
	};

	//@―---------------------------------------------------------------------------
	//! @brief      DirectX12での
	//!				* RootConstants
	//!				* RootDescriptor
	//!				* RootDescriptorTable
	//@―---------------------------------------------------------------------------
	class ShaderResourceGroup {
	public:
		void record(rhi::CommandList& commandList)const {
			// 
		}
	private:

	};

}
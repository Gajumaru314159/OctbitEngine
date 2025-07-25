//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/DescriptorLayoutDesc.h>

namespace ob::rhi {

	//! @brief      デスクリプタレイアウト
	class DescriptorLayout :public GraphicObject {
	public:

		static Ref<DescriptorLayout> Create(const DescriptorLayoutDesc& desc);

	public:

		virtual const DescriptorLayoutDesc& getDesc() const = 0;

	};

}
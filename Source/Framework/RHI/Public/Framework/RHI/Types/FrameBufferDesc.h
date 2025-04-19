//***********************************************************
//! @file
//! @brief		FrameBufferDesc
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/TextureDesc.h>

namespace ob::rhi {

	struct FrameBufferDesc {
		Ref<RenderPass>         renderPass;
		Vector<Ref<Texture>>    attachments;
	};

}
//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPass.h>

namespace ob::graphics {

	struct OutputViewData {
		Ref<rhi::SwapChain>		swapchain;			//!< 描画先
		Ref<rhi::RenderTexture> texture;			//!< 描画先
		Rect                    rect{0,0,1,1};		//!< 描画範囲の矩形
		Viewport				viewport{0,0,1,1};	//!< 描画範囲

		Optional<Size> size()const;
	};

	class CameraPass : public RenderPass {
	public:
		struct Input {
			FGResource color;
		};
		struct Output {
			FGResource color;
		};
	public:

		Output render(FG& fg, RenderView& view, Input input) const;

	};

}
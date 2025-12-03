//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPass.h>

namespace ob::graphics {

	class DeferredPass : public RenderPass {
	public:
		struct Input {
			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
		struct Output {
			FGResource color;

			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
	public:
		DeferredPass();
		Output render(FG& fg, RenderView& view, const Input& input)const;
	private:
		Ref<Mesh> m_mesh;
	};

}
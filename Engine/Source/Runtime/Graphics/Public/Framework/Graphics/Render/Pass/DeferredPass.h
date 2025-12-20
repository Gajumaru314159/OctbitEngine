//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPass.h>

namespace ob::graphics {

	struct DeferredData {
		Ref<Material> material;
	};

	class DeferredPass : public RenderPass {
	public:
		struct Input {
			FGResource albedo;
			FGResource normal;
			FGResource params;
			FGResource depth;
		};
		struct Output : Input{
			FGResource color;
		};
	public:
		DeferredPass();
		Output render(FG& fg, RenderView& view, const Input& input)const;
	private:
		Ref<Mesh> m_mesh;
	};

}
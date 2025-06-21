//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderPass.h>
#include <Framework/Graphics/FrameGraph/FG.h>

namespace ob::graphics {

	class EarlyZPass : public RenderPass {
	public:
		struct Input {
			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
		struct Output {
			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
	public:
		EarlyZPass();
		Output render(FG& fg, RenderView& view,Input input)const;
	};

	class OpaquePass : public RenderPass {
	public:
		struct Input {
			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
		struct Output {
			FGResource albedo;
			FGResource normal;			
			FGResource depth;
		};
	public:
		OpaquePass();
		Output render(FG& fg, RenderView& view, Input input)const;
	};

	class MaskedPass : public RenderPass {
	public:
		struct Input {
			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
		struct Output {
			FGResource albedo;
			FGResource normal;			
			FGResource depth;
		};
	public:
		MaskedPass();
		Output render(FG& fg, RenderView& view, Input input)const;
	};

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
		Output render(FG& fg, RenderView& view, Input input)const;
	private:
		Ref<Material> m_material;
		Ref<Mesh> m_mesh;
	};

}
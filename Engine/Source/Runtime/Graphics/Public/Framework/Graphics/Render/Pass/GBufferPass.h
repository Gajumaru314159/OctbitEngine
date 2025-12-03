//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPass.h>

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
		Output render(FG& fg, RenderView& view, const Input& input)const;
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
		Output render(FG& fg, RenderView& view, const Input& input)const;
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
		Output render(FG& fg, RenderView& view, const Input& input)const;
	};


	class GBufferPass : public RenderPass {
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
		GBufferPass() {}
		Output render(FG& fg, RenderView& view, Input input)const {
			auto earlyZ = m_earlyZ.render(fg, view, { input.albedo, input.normal, input.depth });
			auto opaque = m_opaque.render(fg, view, { earlyZ.albedo , earlyZ.normal, earlyZ.depth });
			auto masked = m_masked.render(fg, view, { opaque.albedo , opaque.normal, opaque.depth });
			return { masked.albedo,masked.normal,masked.depth };
		}
	private:
		EarlyZPass m_earlyZ;
		OpaquePass m_opaque;
		MaskedPass m_masked;
	};

}
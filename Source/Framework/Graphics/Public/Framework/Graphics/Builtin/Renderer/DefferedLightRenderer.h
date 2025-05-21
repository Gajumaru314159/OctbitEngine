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

			void connect(FGConnections& connections) {
				albedo = connections.get("EarlyZPass.albedo");
				normal = connections.get("EarlyZPass.normal");
				depth = connections.get("EarlyZPass.depth");
			}
		};
		struct Output {
			FGResource albedo;
			FGResource normal;
			FGResource depth;

			void connect(FGConnections& connections) {
				connections.set("EarlyZPass.albedo", albedo);
				connections.set("EarlyZPass.normal", normal);
				connections.set("EarlyZPass.depth", depth);
			}
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

			void connect(FGConnections& connections) {
				albedo = connections.get("OpaquePass.albedo");
				normal = connections.get("OpaquePass.normal");
				depth = connections.get("OpaquePass.depth");
			}
		};
		struct Output {
			FGResource albedo;
			FGResource normal;			
			FGResource depth;

			void connect(FGConnections& connections) {
				connections.set("OpaquePass.albedo", albedo);
				connections.set("OpaquePass.normal", normal);
				connections.set("OpaquePass.depth", depth);
			}
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

			void connect(FGConnections& connections) {
				albedo = connections.get("MaskedPass.albedo");
				normal = connections.get("MaskedPass.normal");
				depth = connections.get("MaskedPass.depth");
			}
		};
		struct Output {
			FGResource albedo;
			FGResource normal;			
			FGResource depth;

			void connect(FGConnections& connections) {
				connections.set("MaskedPass.albedo", albedo);
				connections.set("MaskedPass.normal", normal);
				connections.set("MaskedPass.depth", depth);
			}
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

			void connect(FGConnections& connections) {
				albedo = connections.get("DeferredPass.albedo");
				normal = connections.get("DeferredPass.normal");
				depth = connections.get("DeferredPass.depth");
			}
		};
		struct Output {
			FGResource color;

			FGResource albedo;
			FGResource normal;
			FGResource depth;

			void connect(FGConnections& connections) {
				connections.set("DeferredPass.color", color);
				connections.set("DeferredPass.albedo", albedo);
				connections.set("DeferredPass.normal", normal);
				connections.set("DeferredPass.depth", depth);
			}
		};
	public:
		DeferredPass();
		Output render(FG& fg, RenderView& view, Input input)const;
	private:
		Ref<Material> m_material;
		Ref<Mesh> m_mesh;
	};

}
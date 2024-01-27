//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/RHI/CommandList.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	//@―---------------------------------------------------------------------------
	class ImGuiRenderFeature : public RenderFeature{
	public:

		OB_RTTI();

		ImGuiRenderFeature(RenderScene& scene);

		~ImGuiRenderFeature();

		void render(FG& fg, FrameGraphResource target, u32 id);

	private:

		void prepareView(u32 id);

	public:


		struct DrawCommand {
			IntRect		rect;
			ImTextureID texture;
			rhi::DrawIndexedParam param;
		};

		struct DrawContext {
			ImGuiContext* context;
			Array<DrawCommand> commands;

			Ref<rhi::RenderPass> renderPass;
			Ref<rhi::PipelineState> pipeline;
			Ref<rhi::Buffer>   vertexBuffer;
			Ref<rhi::Buffer>   indexBuffer;

			Ref<rhi::Buffer>            constantBuffer;
			Ref<rhi::DescriptorTable>   constantTable;
		};


		Ref<rhi::RootSignature> m_signature;
		Ref<rhi::Shader>		m_vs;
		Ref<rhi::Shader>		m_ps;

		HashMap<u32,DrawContext> m_contextLists;

	};

}
//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>
#include <Framework/RHI/All.h>

namespace ob::graphics {

	ImGuiRenderFeature::ImGuiRenderFeature(RenderScene& scene)
		:RenderFeature(scene)
	{
		using namespace ob::rhi;

		{
			String code;
			code.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
			code.append(TC("Texture2D g_mainTex:register(t0);								\n"));
			code.append(TC("cbuffer vertexBuffer : register(b0) {							\n"));
			code.append(TC("	float4x4 proj;												\n"));
			code.append(TC("};																\n"));
			code.append(TC("// IN / OUT														\n"));
			code.append(TC("struct VsIn {													\n"));
			code.append(TC("  float2 pos	:POSITION;										\n"));
			code.append(TC("  float2 uv	    :TEXCOORD0;										\n"));
			code.append(TC("  float4 color	:COLOR0;										\n"));
			code.append(TC("};																\n"));
			code.append(TC("struct PsIn {													\n"));
			code.append(TC("  float4 pos	:SV_POSITION;									\n"));
			code.append(TC("  float2 uv	    :TEXCOORD0;										\n"));
			code.append(TC("  float4 color	:COLOR0;										\n"));
			code.append(TC("};																\n"));
			code.append(TC("// エントリ														\n"));
			code.append(TC("struct PsOut {													\n"));
			code.append(TC("  float4 color0	:SV_TARGET0;									\n"));
			code.append(TC("  float4 color1	:SV_TARGET1;									\n"));
			code.append(TC("};																\n"));
			code.append(TC("PsIn VS_Main(VsIn i) {											\n"));
			code.append(TC("    PsIn o;														\n"));
			code.append(TC("    o.pos = mul( proj, float4(i.pos.xy, 0, 1));					\n"));
			code.append(TC("    o.color = i.color;								            \n"));
			code.append(TC("    o.uv  = i.uv;				                                \n"));
			code.append(TC("    return o;													\n"));
			code.append(TC("}																\n"));
			code.append(TC("PsOut PS_Main(PsIn i){											\n"));
			code.append(TC("	PsOut o;													\n"));
			code.append(TC("	o.color0 = g_mainTex.Sample(g_mainSampler,i.uv)*i.color;	\n"));
			code.append(TC("	o.color1 = g_mainTex.Sample(g_mainSampler,i.uv)*i.color;	\n"));
			code.append(TC("    return o;											        \n"));
			code.append(TC("}																\n"));

			m_vs = Shader::CompileVS(code);
			m_ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(m_vs && m_ps);
		}


		{
			RootSignatureDesc desc(
				{
					RootParameter::Range(DescriptorRangeType::CBV,1,0),
					RootParameter::Range(DescriptorRangeType::SRV,1,0),
				},
				{
					StaticSamplerDesc(SamplerDesc(),0),
				}
				);
			desc.name = TC("::ImGui");
			m_signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(m_signature);
		}

	}

	ImGuiRenderFeature::~ImGuiRenderFeature() {

		for (auto& [id, context] : m_contextLists) {
			ImGui::DestroyContext(context.context);
		}

	}

	void ImGuiRenderFeature::prepareView(u32 id) {

		using namespace ob::rhi;

		if (m_contextLists.count(id))return;

		auto& context = m_contextLists[id];

		{
			PipelineStateDesc desc;
			desc.name = TC("::ImGui");
			//desc.renderPass = m_rende;
			desc.subpass = 0;

			desc.rootSignature = m_signature;
			desc.vs = m_vs;
			desc.ps = m_ps;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,offsetof(ImDrawVert,pos),Type::Float,2),
				VertexAttribute(Semantic::TexCoord,offsetof(ImDrawVert,uv),Type::Float,2),
				VertexAttribute(Semantic::Color,offsetof(ImDrawVert,col),Type::UInt8,4),
			};
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;

			context.pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(context.pipeline);
		}

	}

	void ImGuiRenderFeature::render(FG& fg, FrameGraphResource target, u32 id) {

		if (m_contextLists.count(id) == 0)return;

		auto& context = m_contextLists[id];

		ImGui::SetCurrentContext(context.context);

		// マウス更新
		// キーボード更新
		// ウィンドウサイズ更新
		// 入力更新

		ImGui::NewFrame();

		ImGui::Render();

		// バッファ更新

		context.commands.clear();

		ImDrawData* drawData = ::ImGui::GetDrawData();

		size_t globalIndexOffset = 0;
		size_t globalVertexOffset = 0;

		for (auto& cmdList : Span<ImDrawList*>(drawData->CmdLists, drawData->CmdListsCount)) {
			for (auto& cmdBuf : Span<ImDrawCmd>(cmdList->CmdBuffer.Data, cmdList->CmdBuffer.Size)) {

				// シザー/クリッピングの矩形をフレームバッファ空間に投影する
				ImVec2 clip = drawData->DisplayPos;
				ImVec2 clip_min(cmdBuf.ClipRect.x - clip.x, cmdBuf.ClipRect.y - clip.y);
				ImVec2 clip_max(cmdBuf.ClipRect.z - clip.x, cmdBuf.ClipRect.w - clip.y);

				DrawCommand cmd;

				cmd.rect = { (s32)clip_min.x, (s32)clip_min.y, (s32)clip_max.x, (s32)clip_max.y };
				if (cmd.rect.empty()) continue;

				cmd.texture = cmdBuf.TextureId;

				cmd.param.indexCount = cmdBuf.ElemCount;
				cmd.param.startIndex = cmdBuf.IdxOffset + globalIndexOffset;
				cmd.param.startVertex = cmdBuf.VtxOffset + globalVertexOffset;

				context.commands.push_back(cmd);

			}
			globalIndexOffset += cmdList->IdxBuffer.Size;
			globalVertexOffset += cmdList->VtxBuffer.Size;
		}


		// バッファコピー

		struct ImGuiData {
			FrameGraphResource target;
		};

		fg.addCallbackPass<ImGuiData>(
			"ImGui",
			[=](FrameGraph::Builder& builder, ImGuiData& data) {
				data.target = builder.write(target);
			},
			[this](const ImGuiData& data, FrameGraphPassResources& resources, void* ctx) {
				auto& cmdList = *static_cast<rhi::CommandList*>(ctx);

				for (auto& [id, context] : m_contextLists) {

					cmdList.beginRenderPass(context.renderPass);
					cmdList.setPipelineState(context.pipeline);

					for (auto& cmd : context.commands) {

						cmdList.drawIndexed();

					}

					cmdList.endRenderPass();
				}
			}
		);
	};

}
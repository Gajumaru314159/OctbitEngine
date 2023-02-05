//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/ImGui/ImGui.h>
#include <Framework/Core/Graphic/Bitmap.h>
#include <Framework/Input/Mouse.h>
#include <Framework/Input/Keyboard.h>
#include <Framework/RHI/All.h>


namespace ob::imgui {

	struct ImGuiData
	{
		ob::platform::Window        window;
		ob::core::DateTime          time;
		ImGuiMouseCursor            cursorType;
		//ob::input::CursorType       cursorType;
		//HWND                        MouseHwnd;
		//bool                        MouseTracked;
		s32                         MouseButtonsDown;
		//INT64                       Time;
		//INT64                       TicksPerSecond;
		//ImGuiMouseCursor            LastMouseCursor;

	};

	static ImGuiData* ImGui_GetBackendData()
	{
		return ImGui::GetCurrentContext() ? (ImGuiData*)ImGui::GetIO().BackendPlatformUserData : nullptr;
	}


	bool ImGui_Init(const ob::platform::Window& window) {

		ImGuiIO& io = ImGui::GetIO();
		OB_ASSERT(io.BackendPlatformUserData == nullptr, "初期化済みです");

		//s64 perf_frequency, perf_counter;
		//if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency))
		//    return false;
		//if (!::QueryPerformanceCounter((LARGE_INTEGER*)&perf_counter))
		//    return false;

		// Setup backend capabilities flags
		auto bd = new ImGuiData();
		io.BackendPlatformUserData = (void*)bd;
		io.BackendPlatformName = "OctbitEngine";
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

		bd->window = window;
		//bd->TicksPerSecond = perf_frequency;
		//bd->Time = perf_counter;
		//bd->LastMouseCursor = ImGuiMouseCursor_COUNT;

		// Set platform dependent data in viewport
		ImGui::GetMainViewport()->PlatformHandleRaw = window.getHandle();

		return true;
	}
	void ImGui_Shutdown() {
		ImGuiData* bd = ImGui_GetBackendData();
		OB_ASSERT(bd != nullptr, "解放済みです");
		ImGuiIO& io = ImGui::GetIO();

		io.BackendPlatformName = nullptr;
		io.BackendPlatformUserData = nullptr;
		delete bd;
	}

	static void ImGui_UpdateMouse()
	{
		ImGuiData* bd = ImGui_GetBackendData();
		ImGuiIO& io = ImGui::GetIO();

		if (!bd->window)
			return;

		const bool focused = bd->window.getState()[platform::WindowState::Focused];
		if (focused)
		{
			// (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
			if (io.WantSetMousePos)
			{
				OB_NOTIMPLEMENTED();
				auto pos = bd->window.getScreenPoint({ (s32)io.MousePos.x, (s32)io.MousePos.y });
				//input::Mouse::SetPos(pos);
			}

			// (Optional) Fallback to provide mouse position when focused (WM_MOUSEMOVE already provides this when hovered or captured)
			if (!io.WantSetMousePos)
			{
				auto pos = input::Mouse::GetPos();
				auto cpos = bd->window.getClientPoint(Point{ (s32)pos.x,(s32)pos.y});
				io.AddMousePosEvent((float)cpos.x, (float)cpos.y);
			}

			s32 button = -1;
			if (input::Mouse::Left.down())button = 0;
			if (input::Mouse::Right.down())button = 1;
			if (input::Mouse::Middle.down())button = 2;

			if (button != -1) {
				bd->MouseButtonsDown |= 1 << button;
				io.AddMouseButtonEvent(button, true);
			}

			button = -1;
			if (input::Mouse::Left.up())button = 0;
			if (input::Mouse::Right.up())button = 1;
			if (input::Mouse::Middle.up())button = 2;

			if (button != -1) {
				bd->MouseButtonsDown &= ~(1 << button);
				io.AddMouseButtonEvent(button, false);
			}

			auto wheel = input::Mouse::Wheel.value();
			if (wheel != 0.0f) {
				io.AddMouseWheelEvent(0.0f, wheel);
			}
		}
	}


	static void ImGui_AddKeyEvent(ImGuiKey imkey, bool down, input::Key key, int native_scancode = -1)
	{
		OB_UNUSED(native_scancode);
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(imkey, down);
		io.SetKeyEventNativeData(imkey, enum_cast(key), native_scancode);
	}

	static void ImGui_UpdateKeyboard() {

		using namespace ob::input;

		// Left & right Shift keys: when both are pressed together, Windows tend to not generate the WM_KEYUP event for the first released one.
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && !Keyboard::LeftShilt.down())
			ImGui_AddKeyEvent(ImGuiKey_LeftShift, false, Key::LeftShilt);
		if (ImGui::IsKeyDown(ImGuiKey_RightShift) && !Keyboard::RightShift.down())
			ImGui_AddKeyEvent(ImGuiKey_RightShift, false, Key::RightShift);

		// Sometimes WM_KEYUP for Win key is not passed down to the app (e.g. for Win+V on some setups, according to GLFW).
		if (ImGui::IsKeyDown(ImGuiKey_LeftSuper) && !Keyboard::LeftWindows.down())
			ImGui_AddKeyEvent(ImGuiKey_LeftSuper, false, Key::LeftWindows);
		if (ImGui::IsKeyDown(ImGuiKey_RightSuper) && !Keyboard::RightWindows.down())
			ImGui_AddKeyEvent(ImGuiKey_RightSuper, false, Key::RightWindows);

		if (Keyboard::A.up())ImGui_AddKeyEvent(ImGuiKey_A, false, Key::A);

	}

	static bool ImGui_UpdateMouseCursor()
	{
		using namespace ob::input;
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
			return false;

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			//Mouse::SetCursor(CursorType::None);
		} else
		{
			//CursorType type;
			//switch (imgui_cursor)
			//{
			//case ImGuiMouseCursor_Arrow:        type = CursorType::Arrow; break;
			//case ImGuiMouseCursor_TextInput:    type = CursorType::IBEAM; break;
			//case ImGuiMouseCursor_ResizeAll:    type = CursorType::SizeAll; break;
			//case ImGuiMouseCursor_ResizeEW:     type = CursorType::SizeWE; break;
			//case ImGuiMouseCursor_ResizeNS:     type = CursorType::SizeNS; break;
			//case ImGuiMouseCursor_ResizeNESW:   type = CursorType::SizeNESW; break;
			//case ImGuiMouseCursor_ResizeNWSE:   type = CursorType::SizeNWSE; break;
			//case ImGuiMouseCursor_Hand:         type = CursorType::Hand; break;
			//case ImGuiMouseCursor_NotAllowed:   type = CursorType::No; break;
			//}
			//Mouse::SetCursor(type);
		}
		return true;
	}

	void ImGui_NewFrame() {
		ImGuiIO& io = ImGui::GetIO();
		ImGuiData* bd = ImGui_GetBackendData();
		OB_ASSERT(bd != nullptr, "初期化されていません。ImGui_Init()の呼び出しを確認してください。");

		// Setup display size (every frame to accommodate for window resizing)
		auto size = bd->window.getSize();
		io.DisplaySize = ImVec2(size.width, size.height);

		// Setup time step
		auto now = DateTime::Now();
		if (bd->time == DateTime()) {
			io.DeltaTime = 1.0f / 60.0f;
		} else {
			io.DeltaTime = abs(TimeSpan(now, bd->time).secondsF());
		}
		bd->time = now;

		// Update OS mouse position
		ImGui_UpdateMouse();

		// Process workarounds for known Windows key handling issues
		ImGui_UpdateKeyboard();

		// Update OS mouse cursor with the cursor requested by imgui
		ImGuiMouseCursor cursorType = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
		if (bd->cursorType != cursorType)
		{
			bd->cursorType = cursorType;
			ImGui_UpdateMouseCursor();
		}

	}

	//void     ImGui_EnableDpiAwareness();
	//float    ImGui_GetDpiScaleForHwnd(void* hwnd);       // HWND hwnd
	//float    ImGui_GetDpiScaleForMonitor(void* monitor); // HMONITOR monitor
	//
	//void     ImGui_EnableAlphaCompositing(void* hwnd);   // HWND hwnd

	struct ImGuiDrawData
	{
		u64                         vertexCount = 5000;
		u64                         indexCount = 10000;
		Ref<rhi::Buffer>            vertexBuffer;
		Ref<rhi::Buffer>            indexBuffer;
		Ref<rhi::Texture>           fontTexture;

		Ref<rhi::Buffer>            constantBuffer;
		Ref<rhi::DescriptorTable>   constantTable;

		Ref<rhi::RootSignature>     signature;
		Ref<rhi::PipelineState>     pipeline;
		Ref<rhi::DescriptorTable>   fontTextureTable;
	};

	static ImGuiDrawData* ImGuiGeDrawData()
	{
		return ImGui::GetCurrentContext() ? (ImGuiDrawData*)ImGui::GetIO().BackendRendererUserData : nullptr;
	}


	bool ImGui_DrawInit(const Ref<rhi::RenderPass>& renderPass, s32 subpass) {
		using namespace ob::rhi;

		ImGuiIO& io = ImGui::GetIO();
		OB_ASSERT(io.BackendRendererUserData == nullptr, "初期化済みです。");

		// Setup backend capabilities flags
		auto bd = new ImGuiDrawData();
		io.BackendRendererUserData = (void*)bd;
		io.BackendRendererName = "OctbitEngine";
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.

		Ref<Shader> vs;
		Ref<Shader> ps;
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
			code.append(TC("	o.color0 = g_mainTex.Sample(g_mainSampler,i.uv).a*i.color;	\n"));
			code.append(TC("	o.color1 = g_mainTex.Sample(g_mainSampler,i.uv).a*i.color;	\n"));
			code.append(TC("    return o;											        \n"));
			code.append(TC("}																\n"));

			vs = VertexShader::Create(code);
			ps = PixelShader::Create(code);
			OB_ASSERT_EXPR(vs && ps);
		}

		Ref<RootSignature> signature;
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
			desc.name = TC("ImGui");
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}

		{
			PipelineStateDesc desc;
			desc.name = TC("ImGui");
			desc.renderPass = renderPass;
			desc.subpass = subpass;

			desc.rootSignature = signature;
			desc.vs = vs;
			desc.ps = ps;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,offsetof(ImDrawVert,pos),Type::Float,2),
				VertexAttribute(Semantic::TexCoord,offsetof(ImDrawVert,uv),Type::Float,2),
				VertexAttribute(Semantic::Color,offsetof(ImDrawVert,col),Type::UInt8,4),
			};
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.blend[1] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
			desc.depthStencil.stencil.enable = false;

			bd->pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(bd->pipeline);
		}

		{
			auto desc = BufferDesc::Constant(sizeof(Matrix), BindFlag::AllShaderResource);
			bd->constantBuffer = Buffer::Create(desc);
			OB_ASSERT_EXPR(bd->constantBuffer);

			bd->constantTable = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
			bd->constantTable->setResource(0, bd->constantBuffer);
		}

		return true;
	}


	void ImGui_InvalidateDeviceObjects()
	{
		ImGuiDrawData* bd = ImGuiGeDrawData();
		if (bd->vertexBuffer) {
			bd->vertexBuffer.reset();
		}
		if (bd->indexBuffer) {
			bd->indexBuffer.reset();
		}
		if (bd->fontTexture) {
			bd->fontTexture.reset();
			ImGui::GetIO().Fonts->SetTexID(nullptr);
		}
	}

	void ImGui_DrawShutdown() {
		ImGuiDrawData* bd = ImGuiGeDrawData();
		OB_ASSERT(bd != nullptr , "解放済み。");
		ImGuiIO& io = ImGui::GetIO();

		ImGui_InvalidateDeviceObjects();
		io.BackendRendererName = nullptr;
		io.BackendRendererUserData = nullptr;
		delete bd;
	}


	static bool ImGui_CreateFontsTexture()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiDrawData* bd = ImGuiGeDrawData();
		unsigned char* pixels;

		// テクスチャサイズ取得
		int width, height, bytes_per_pixel;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

		// Convert RGBA32 to BGRA32 (because RGBA32 is not well supported by DX9 devices)
//#ifndef IMGUI_USE_BGRA_PACKED_COLOR
//        if (io.Fonts->TexPixelsUseColors)
//        {
//            ImU32* dst_start = (ImU32*)ImGui::MemAlloc((size_t)width * height * bytes_per_pixel);
//            for (ImU32* src = (ImU32*)pixels, *dst = dst_start, *dst_end = dst_start + (size_t)width * height; dst < dst_end; src++, dst++)
//                *dst = IMGUI_COL_TO_DX9_ARGB(*src);
//            pixels = (unsigned char*)dst_start;
//        }
//#endif

		// Upload texture to graphics system
		using namespace ob::rhi;
		rhi::TextureDesc desc;
		desc.name = TC("ImGui_FontTexture");
		desc.size = { width,height,0 };
		desc.format = TextureFormat::RGBA8;
		desc.type = TextureType::Texture2D;


		//
		Array<IntColor> colors(width*height);
		for (s32 y = 0; y < height; y++) {
			for (s32 x = 0; x < width; x++) {
				size_t index = ((size_t)width * y + x);
				size_t index2 = ((size_t)width * y + x) * bytes_per_pixel;
				IntColor color;
				color.r = pixels[index2 + 0];
				color.g = pixels[index2 + 1];
				color.b = pixels[index2 + 2];
				color.a = pixels[index2 + 3];
				colors[index] = color;
			}
		}


		bd->fontTexture = rhi::Texture::Create(Size(width,height), colors);
		if (!bd->fontTexture)
			return false;

		bd->fontTextureTable = rhi::DescriptorTable::Create(rhi::DescriptorHeapType::CBV_SRV_UAV, 1);
		bd->fontTextureTable->setResource(0,bd->fontTexture);

		// Store our identifier
		io.Fonts->SetTexID((ImTextureID)&bd->fontTextureTable);

		return true;
	}


	bool ImGui_CreateDeviceObjects() {
		ImGuiDrawData* bd = ImGuiGeDrawData();
		if (!bd)
			return false;
		if (!ImGui_CreateFontsTexture())
			return false;
		return true;
	}

	void ImGui_DrawNewFrame() {
		ImGuiDrawData* bd = ImGuiGeDrawData();
		OB_ASSERT(bd != nullptr, "初期化されていません。ImGui_Init()の呼び出しを確認してください。");

		if (!bd->fontTexture)
			ImGui_CreateDeviceObjects();
	}

	static void ImGui_SetupRenderState(ImDrawData* draw_data,const Ref<rhi::CommandList>& commandList) {
		ImGuiDrawData* bd = ImGuiGeDrawData();

		{
			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

			auto mvp = Matrix::Orthographic(L, R, B, T, 0, 1);

			bd->constantBuffer->updateDirect(mvp,0);
		}

		// Setup viewport
		Viewport vp(0, 0, draw_data->DisplaySize.x, draw_data->DisplaySize.y, 0, 1);
		commandList->setViewport(&vp,1);

		// Bind shader and vertex buffers
		unsigned int stride = sizeof(ImDrawVert);
		unsigned int offset = 0;

		commandList->setVertexBuffer(bd->vertexBuffer);
		commandList->setIndexBuffer(bd->indexBuffer);

		commandList->setPipelineState(bd->pipeline);

		{
			rhi::SetDescriptorTableParam params[] = {
				{bd->constantTable,0}
			};
			commandList->setRootDesciptorTable(params, std::size(params));
		}

	}

	void ImGui_RenderDrawData(const Ref<rhi::CommandList>& commandList) {

		using namespace ob::rhi;

		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGuiDrawData* bd = ImGuiGeDrawData();

		if (!draw_data) {
			return;
		}
		if (!bd) {
			return;
		}

		// ディスプレイサイズが0の場合はスキップ
		if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
			return;

		// バッファ生成
		if (!bd->vertexBuffer || bd->vertexCount < draw_data->TotalVtxCount)
		{
			bd->vertexCount = draw_data->TotalVtxCount + 5000;

			BufferDesc desc = BufferDesc::Vertex<ImDrawVert>(bd->vertexCount);
			desc.name = TC("ImGui");
			bd->vertexBuffer = Buffer::Create(desc);

			if (bd->vertexBuffer)
				return;
		}
		if (!bd->indexBuffer || bd->indexCount < draw_data->TotalIdxCount)
		{
			bd->indexCount = draw_data->TotalIdxCount + 10000;

			BufferDesc desc = BufferDesc::Index<ImDrawIdx>(bd->indexCount);
			desc.name = TC("ImGui");
			bd->indexBuffer = Buffer::Create(desc);

			if (bd->indexBuffer)
				return;
		}

		// バッファコピー
		bd->vertexBuffer->updateDirect(
			[&](void* ptr) {
				ImDrawVert* vertices = reinterpret_cast<ImDrawVert*>(ptr);
				for (int n = 0; n < draw_data->CmdListsCount; n++)
				{
					const ImDrawList* cmd_list = draw_data->CmdLists[n];
					memcpy(vertices, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
					vertices += cmd_list->VtxBuffer.Size;
				}
			}
		);
		bd->indexBuffer->updateDirect(
			[&](void* ptr) {
				ImDrawIdx* indices = reinterpret_cast<ImDrawIdx*>(ptr);
				for (int n = 0; n < draw_data->CmdListsCount; n++)
				{
					const ImDrawList* cmd_list = draw_data->CmdLists[n];
					memcpy(indices, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
					indices += cmd_list->IdxBuffer.Size;
				}
			}
		);

		ImGui_SetupRenderState(draw_data,commandList);

		// Render command lists
		// (Because we merged all buffers into a single one, we maintain our own offset into them)
		int global_vtx_offset = 0;
		int global_idx_offset = 0;
		ImVec2 clip_off = draw_data->DisplayPos;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback != nullptr)
				{
					// ImDrawList::AddCallback() を介して登録されたユーザーコールバック。
					// ImDrawCallback_ResetRenderState は、レンダリング状態をリセットするようにレンダラーに要求するためにユーザーが使用する特別なコールバック値です。
					if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) {
						ImGui_SetupRenderState(draw_data, commandList);
					} else {
						pcmd->UserCallback(cmd_list, pcmd);
					}
				} else {
					// Project scissor/clipping rectangles into framebuffer space
					ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
					ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
					if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
						continue;

					// Apply Scissor/clipping rectangle, Bind texture, Draw
					IntRect rect = { (s32)clip_min.x, (s32)clip_min.y, (s32)clip_max.x, (s32)clip_max.y };
					commandList->setScissorRect(&rect, 1);

					{
						SetDescriptorTableParam params[] = {
							{*(Ref<DescriptorTable>*)pcmd->TextureId,1}
						};
						commandList->setRootDesciptorTable(params, std::size(params));
					}

					{
						DrawIndexedParam param;
						param.indexCount = pcmd->ElemCount;
						param.startIndex = pcmd->IdxOffset + global_idx_offset;
						param.startVertex = pcmd->VtxOffset + global_vtx_offset;

						commandList->drawIndexed(param);
					}

				}
			}
			global_idx_offset += cmd_list->IdxBuffer.Size;
			global_vtx_offset += cmd_list->VtxBuffer.Size;
		}

	}


}// namespcae ob

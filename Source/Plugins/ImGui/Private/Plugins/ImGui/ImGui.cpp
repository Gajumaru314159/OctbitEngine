﻿//***********************************************************
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


// テスト
#include <Windows.h>


ImFileHandle ImFileOpen(const char* filename, const char* mode) {
	using namespace ob::core;
	BitFlags<FileOpenMode> modes;
	if (strchr(mode, 'r'))modes.on(FileOpenMode::Read);
	if (strchr(mode, 'w'))modes.on(FileOpenMode::Write);
	if (strchr(mode, 'a'))modes.on(FileOpenMode::Append);
	if (strchr(mode, 'b')==nullptr)modes.on(FileOpenMode::Text);
	if (strchr(mode, '+'))LOG_FATAL("Not supported");// modes.on(FileOpenMode::Append);
	Path path = filename;
	return new ob::core::FileStream(path, modes.get_enum());
}
bool ImFileClose(ImFileHandle file) {
	if (file!=nullptr)delete file;
	return file!=nullptr;
}
ob::u64 ImFileGetSize(ImFileHandle file) {
	return file ? file->size() : 0;
}
ob::u64 ImFileRead(void* data, ob::u64 size, ob::u64 count, ImFileHandle file) {
	if (!file)return 0;
	if (file->canRead() == false)return 0;
	file->read(data, size * count);
	return size * count;
}
ImU64 ImFileWrite(const void* data, ob::u64 size, ob::u64 count, ImFileHandle file) {
	if (!file)return 0;
	if (file->canWrite() == false)return 0;
	file->write(data, size * count);
	return size * count;
}


namespace ImGui {
	inline namespace ob {

		struct BackendData
		{
			ob::platform::Window        window;
			ob::core::DateTime          time;
			ImGuiMouseCursor            cursorType;

			ImString					stringConvertBuffer;
			Array<char>					stringBuffer;
			size_t						stringIndex;

			ImString					clipboardBuffer;
		};

		struct DrawData
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

		//@―---------------------------------------------------------------------------
		//! @brief      バックエンドデータ取得
		//@―---------------------------------------------------------------------------
		static BackendData* GetObBackendData()
		{
			return ::ImGui::GetCurrentContext() ? (BackendData*)::ImGui::GetIO().BackendPlatformUserData : nullptr;
		}

		//@―---------------------------------------------------------------------------
		//! @brief      描画データ取得(描画用)
		//@―---------------------------------------------------------------------------
		static DrawData* GetObDrawData()
		{
			return ::ImGui::GetCurrentContext() ? (DrawData*)::ImGui::GetIO().BackendRendererUserData : nullptr;
		}


		//@―---------------------------------------------------------------------------
		//! @brief      グラフィックリソース初期化
		//@―---------------------------------------------------------------------------
		bool InitDrawResource(const Ref<rhi::RenderPass>& renderPass, s32 subpass) {
			using namespace ob::rhi;

			ImGuiIO& io = ::ImGui::GetIO();
			OB_ASSERT(io.BackendRendererUserData == nullptr, "初期化済みです。");

			// Setup backend capabilities flags
			auto bd = new DrawData();
			io.BackendRendererUserData = (void*)bd;
			io.BackendRendererName = "OctbitEngine";
			io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // 大きなメッシュを使用できるようにする

			FileStream file(TC("Asset/Font/ipaexg.ttf"));
			Blob blob(file);
			if (blob) {
				ImFontConfig config;
				config.OversampleH = 1;
				config.RasterizerMultiply = 1.2f;
				void* ptr = ::ImGui::MemAlloc(blob.size());
				memcpy_s(ptr, blob.size(), blob.data(), blob.size());
				io.Fonts->AddFontFromMemoryTTF(ptr, blob.size(), 13.0f, &config, io.Fonts->GetGlyphRangesJapanese());
			}

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
				desc.name = TC("::ImGui");
				signature = RootSignature::Create(desc);
				OB_ASSERT_EXPR(signature);
			}

			{
				PipelineStateDesc desc;
				desc.name = TC("::ImGui");
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
				desc.rasterizer.cullMode = CullMode::None;

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


		//@―---------------------------------------------------------------------------
		//! @brief      マウスカーソル更新
		//@―---------------------------------------------------------------------------
		static void UpdateMouseCursor()
		{
			using namespace ob::input;
			ImGuiIO& io = ::ImGui::GetIO();
			auto bd = GetObBackendData();

			if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
				return;

			if (!bd)
				return;

			// カーソル変更時に変更
			ImGuiMouseCursor cursorType = io.MouseDrawCursor ? ImGuiMouseCursor_None : ::ImGui::GetMouseCursor();
			if (bd->cursorType != cursorType)
			{
				bd->cursorType = cursorType;

				ImGuiMouseCursor imgui_cursor = ::ImGui::GetMouseCursor();
				if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
				{
					// Hide OS mouse cursor if ::ImGui is drawing it or if it wants no cursor
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
			}
		}


		//@―---------------------------------------------------------------------------
		//! @brief      マウス更新
		//@―---------------------------------------------------------------------------
		static void UpdateMouse()
		{
			using namespace ob::input;
			ImGuiIO& io = ::ImGui::GetIO();
			BackendData* bd = GetObBackendData();

			if (bd == nullptr || !bd->window)
				return;

			// ウィンドウフォーカス中の未処理
			const bool focused = bd->window.getState()[platform::WindowState::Focused];
			if (focused)
			{
				// カーソル位置更新
				auto pos = input::Mouse::GetPos();
				auto cpos = bd->window.getClientPoint({ pos.x,pos.y });
				io.AddMousePosEvent((float)cpos.x, (float)cpos.y);


				// ボタン入力
				if (input::Mouse::Left.down())
					io.AddMouseButtonEvent(0, true);
				else if (input::Mouse::Left.up())
					io.AddMouseButtonEvent(0, false);

				if (input::Mouse::Right.down())
					io.AddMouseButtonEvent(1, true);
				else if (input::Mouse::Right.up())
					io.AddMouseButtonEvent(1, false);

				if (input::Mouse::Middle.down())
					io.AddMouseButtonEvent(2, true);
				else if (input::Mouse::Middle.up())
					io.AddMouseButtonEvent(2, false);

				// ホイール
				auto wheelX = input::Mouse::WheelX.value();
				auto wheelY = input::Mouse::Wheel.value();
				if (wheelX != 0.0f) {
					io.AddMouseWheelEvent(wheelX, 0);
				}
				if (wheelY != 0.0f) {
					io.AddMouseWheelEvent(0, wheelY * 0.01f);
				}
			}
		}


		//@―---------------------------------------------------------------------------
		//! @brief      キーボード更新
		//@―---------------------------------------------------------------------------
		static void UpdateKeyboard() {

			using namespace ob::input;

			ImGuiIO& io = ::ImGui::GetIO();

			// 同時押し対応？
			// Left & right Shift keys: when both are pressed together, Windows tend to not generate the WM_KEYUP event for the first released one.
			//if (::ImGui::IsKeyDown(ImGuiKey_LeftShift) && !Keyboard::LeftShift.down())
			//	io.AddKeyEvent(ImGuiKey_LeftShift, false, Key::LeftShift);
			//if (::ImGui::IsKeyDown(ImGuiKey_RightShift) && !Keyboard::RightShift.down())
			//	io.AddKeyEvent(ImGuiKey_RightShift, false, Key::RightShift);
			//
			//// Sometimes WM_KEYUP for Win key is not passed down to the app (e.g. for Win+V on some setups, according to GLFW).
			//if (::ImGui::IsKeyDown(ImGuiKey_LeftSuper) && !Keyboard::LeftWindows.down())
			//	io.AddKeyEvent(ImGuiKey_LeftSuper, false, Key::LeftWindows);
			//if (::ImGui::IsKeyDown(ImGuiKey_RightSuper) && !Keyboard::RightWindows.down())
			//	io.AddKeyEvent(ImGuiKey_RightSuper, false, Key::RightWindows);


			struct KeyMap {
				Key from;
				ImGuiKey to;
			};

			static const KeyMap keyMap[]{
				{Key::Tab,            ImGuiKey_Tab},
				{Key::LeftArrow,      ImGuiKey_LeftArrow},
				{Key::RightArrow,     ImGuiKey_RightArrow},
				{Key::UpArrow,        ImGuiKey_UpArrow},
				{Key::DownArrow,      ImGuiKey_DownArrow},
				{Key::PageUp,         ImGuiKey_PageUp},
				{Key::PageDown,       ImGuiKey_PageDown},
				{Key::Home,           ImGuiKey_Home},
				{Key::End,            ImGuiKey_End},
				{Key::Insert,         ImGuiKey_Insert},
				{Key::Delete,         ImGuiKey_Delete},
				{Key::Backspace,      ImGuiKey_Backspace},
				{Key::Space,          ImGuiKey_Space},
				{Key::Enter,          ImGuiKey_Enter},
				{Key::Escape,         ImGuiKey_Escape},
				{Key::LeftCtrl,       ImGuiKey_LeftCtrl},
				{Key::LeftShift,      ImGuiKey_LeftShift},
				{Key::LeftAlt,        ImGuiKey_LeftAlt},
				//{Key::LeftSuper,      ImGuiKey_LeftSuper},
				{Key::RightCtrl,      ImGuiKey_RightCtrl},
				{Key::RightShift,     ImGuiKey_RightShift},
				{Key::RightAlt,       ImGuiKey_RightAlt},
				//{Key::RightSuper,     ImGuiKey_RightSuper},
				//{Key::Menu,           ImGuiKey_Menu},
				{Key::Alpha0,         ImGuiKey_0},
				{Key::Alpha1,         ImGuiKey_1},
				{Key::Alpha2,         ImGuiKey_2},
				{Key::Alpha3,         ImGuiKey_3},
				{Key::Alpha4,         ImGuiKey_4},
				{Key::Alpha5,         ImGuiKey_5},
				{Key::Alpha6,         ImGuiKey_6},
				{Key::Alpha7,         ImGuiKey_7},
				{Key::Alpha8,         ImGuiKey_8},
				{Key::Alpha9,         ImGuiKey_9},
				{Key::A,              ImGuiKey_A},
				{Key::B,              ImGuiKey_B},
				{Key::C,              ImGuiKey_C},
				{Key::D,              ImGuiKey_D},
				{Key::E,              ImGuiKey_E},
				{Key::F,              ImGuiKey_F},
				{Key::G,              ImGuiKey_G},
				{Key::H,              ImGuiKey_H},
				{Key::I,              ImGuiKey_I},
				{Key::J,              ImGuiKey_J},
				{Key::K,              ImGuiKey_K},
				{Key::L,              ImGuiKey_L},
				{Key::M,              ImGuiKey_M},
				{Key::N,              ImGuiKey_N},
				{Key::O,              ImGuiKey_O},
				{Key::P,              ImGuiKey_P},
				{Key::Q,              ImGuiKey_Q},
				{Key::R,              ImGuiKey_R},
				{Key::S,              ImGuiKey_S},
				{Key::T,              ImGuiKey_T},
				{Key::U,              ImGuiKey_U},
				{Key::V,              ImGuiKey_V},
				{Key::W,              ImGuiKey_W},
				{Key::X,              ImGuiKey_X},
				{Key::Y,              ImGuiKey_Y},
				{Key::Z,              ImGuiKey_Z},
				{Key::F1,             ImGuiKey_F1},
				{Key::F2,             ImGuiKey_F2},
				{Key::F3,             ImGuiKey_F3},
				{Key::F4,             ImGuiKey_F4},
				{Key::F5,             ImGuiKey_F5},
				{Key::F6,             ImGuiKey_F6},
				{Key::F7,             ImGuiKey_F7},
				{Key::F8,             ImGuiKey_F8},
				{Key::F9,             ImGuiKey_F9},
				{Key::F10,            ImGuiKey_F10},
				{Key::F11,            ImGuiKey_F11},
				{Key::F12,            ImGuiKey_F12},
				{Key::Quote,		  ImGuiKey_Apostrophe},
				{Key::Comma,          ImGuiKey_Comma},
				{Key::Minus,          ImGuiKey_Minus},
				{Key::Period,         ImGuiKey_Period},
				{Key::Slash,          ImGuiKey_Slash},
				{Key::Semicolon,      ImGuiKey_Semicolon},
				{Key::Equals,         ImGuiKey_Equal},
				{Key::LeftBracket,    ImGuiKey_LeftBracket},
				{Key::Backslash,      ImGuiKey_Backslash},
				{Key::RightBracket,   ImGuiKey_RightBracket},
				{Key::BackQuote,	  ImGuiKey_GraveAccent},
				{Key::CapsLock,       ImGuiKey_CapsLock},
				{Key::ScrollLock,     ImGuiKey_ScrollLock},
				{Key::NumLock,        ImGuiKey_NumLock},
				{Key::PrintScreen,    ImGuiKey_PrintScreen},
				{Key::Pause,          ImGuiKey_Pause},
				{Key::Keypad0,        ImGuiKey_Keypad0},
				{Key::Keypad1,        ImGuiKey_Keypad1},
				{Key::Keypad2,        ImGuiKey_Keypad2},
				{Key::Keypad3,        ImGuiKey_Keypad3},
				{Key::Keypad4,        ImGuiKey_Keypad4},
				{Key::Keypad5,        ImGuiKey_Keypad5},
				{Key::Keypad6,        ImGuiKey_Keypad6},
				{Key::Keypad7,        ImGuiKey_Keypad7},
				{Key::Keypad8,        ImGuiKey_Keypad8},
				{Key::Keypad9,        ImGuiKey_Keypad9},
				//{Key::KeypadDecimal,  ImGuiKey_KeypadDecimal},
				{Key::KeypadDivide,   ImGuiKey_KeypadDivide},
				{Key::KeypadMultiply, ImGuiKey_KeypadMultiply},
				{Key::KeypadSubstract,ImGuiKey_KeypadSubtract},
				{Key::KeypadAdd,      ImGuiKey_KeypadAdd},
				{Key::KeypadEnter,    ImGuiKey_KeypadEnter},
				//{Key::KeypadEquals,   ImGuiKey_KeypadEqual},


				{ Key::LeftCtrl,       ImGuiMod_Ctrl },
				{ Key::LeftAlt,       ImGuiMod_Alt },
				{ Key::LeftShift,       ImGuiMod_Shift },
			};

			for (auto& [from, to] : keyMap) {
				if (Keyboard::GetButton(from).down())io.AddKeyEvent(to, true);
				if (Keyboard::GetButton(from).up())io.AddKeyEvent(to, false);
			}

			// 
		}

		//@―---------------------------------------------------------------------------
		//! @brief      IME更新
		//@―---------------------------------------------------------------------------
		static void UpdateImeData(ImGuiViewport*, ImGuiPlatformImeData* data)
		{
			if (data->WantVisible) {
				// TODO ウィンドウごとにIMEコンテキストが異なる
				HIMC hIMC = ImmGetContext((HWND)platform::Window::Main().getHandle());
				COMPOSITIONFORM d;
				d.dwStyle = CFS_POINT;
				d.ptCurrentPos.x = data->InputPos.x;
				d.ptCurrentPos.y = data->InputPos.y;
				ImmSetCompositionWindow(hIMC, &d);
			} else {

			}

			//if (data->WantVisible)
			//{
			//	SDL_Rect r;
			//	r.x = (int)data->InputPos.x;
			//	r.y = (int)data->InputPos.y;
			//	r.w = 1;
			//	r.h = (int)data->InputLineHeight;
			//	SDL_SetTextInputRect(&r);
			//	SDL_StartTextInput();
			//} else
			//{
			//	SDL_StopTextInput();
			//}
		}


		//@―---------------------------------------------------------------------------
		//! @brief      クリップボードにテキストをコピー
		//@―---------------------------------------------------------------------------
		static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
		{
			HGLOBAL hText;

			StringBase<char> text8 = text? text:"";
			WString wtext;
			StringEncoder::Encode(text8, wtext);

			auto bufferSize = sizeof(wchar_t)*(wtext.size() + 1);

			hText = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, bufferSize);
			auto pText = static_cast<wchar_t*>(GlobalLock(hText));

			memcpy_s(pText,bufferSize, wtext.data(), bufferSize);

			GlobalUnlock(hText);

			OpenClipboard(NULL);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hText);
			CloseClipboard();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      クリップボードからテキストを取得
		//@―---------------------------------------------------------------------------
		static const char* ImGui_ImplSDL2_GetClipboardText(void*)
		{
			if (auto bd = GetObBackendData()) {
				if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {

					::OpenClipboard(NULL);

					HANDLE hData;
					hData = ::GetClipboardData(CF_UNICODETEXT);

					wchar_t* pText = (wchar_t*)GlobalLock(hData);

					WString wtext = pText? pText:L"";
					StringEncoder::Encode(wtext,bd->clipboardBuffer);

					GlobalUnlock(hData);

					::CloseClipboard();

				}
				return bd->clipboardBuffer.c_str();
			}
			return "Pasted";
		}

		//@―---------------------------------------------------------------------------
		//! @brief      フォント画像生成
		//@―---------------------------------------------------------------------------
		static bool CreateFontsTexture()
		{
			ImGuiIO& io = ::ImGui::GetIO();
			DrawData* bd = GetObDrawData();
			unsigned char* pixels;

			// テクスチャサイズ取得
			int width, height, bytes_per_pixel;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

			// カラー配列
			Array<IntColor> colors((u64)width * height);
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

			// 生成
			bd->fontTexture = rhi::Texture::Create(Size(width, height), colors);
			if (!bd->fontTexture)
				return false;

			bd->fontTextureTable = rhi::DescriptorTable::Create(rhi::DescriptorHeapType::CBV_SRV_UAV, 1);
			bd->fontTextureTable->setResource(0, bd->fontTexture);

			// システム登録
			io.Fonts->SetTexID((ImTextureID)&bd->fontTextureTable);

			return true;
		}

		//@―---------------------------------------------------------------------------
		//! @brief		描画ステートを設定    
		//@―---------------------------------------------------------------------------
		static void SetupRenderState(ImDrawData* draw_data, const Ref<rhi::CommandList>& commandList) {
			DrawData* bd = GetObDrawData();

			// ビューポート
			{
				float L = draw_data->DisplayPos.x;
				float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
				float T = draw_data->DisplayPos.y;
				float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

				auto mvp = Matrix::Orthographic(L, R, B, T, 0, 1);

				bd->constantBuffer->updateDirect(mvp, 0);
			}

			Viewport vp(0, 0, draw_data->DisplaySize.x, draw_data->DisplaySize.y, 0, 1);
			commandList->setViewport(&vp, 1);

			// プリミティブ
			commandList->setVertexBuffer(bd->vertexBuffer);
			commandList->setIndexBuffer(bd->indexBuffer);

			// パイプライン
			commandList->setPipelineState(bd->pipeline);

			{
				rhi::SetDescriptorTableParam params[] = {
					{bd->constantTable,0}
				};
				commandList->setRootDesciptorTable(params, std::size(params));
			}

		}







		//@―---------------------------------------------------------------------------
		//! @brief      初期化
		//@―---------------------------------------------------------------------------
		bool Startup(const ob::platform::Window& window, const Ref<rhi::RenderPass>& renderPass, s32 subpass) {

			//::ImGui::SetAllocatorFunctions()

			::ImGui::CreateContext();

			ImGuiIO& io = ::ImGui::GetIO();
			OB_ASSERT(io.BackendPlatformUserData == nullptr, "初期化済みです");

			auto bd = new BackendData();
			bd->window = window;
			bd->stringBuffer.resize(10000);

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			io.BackendPlatformUserData = (void*)bd;
			io.BackendPlatformName = "OctbitEngine";
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;	// GetMouseCursor()の値を尊重することができる(オプション)

			io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
			io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
			io.ClipboardUserData = nullptr;
			io.SetPlatformImeDataFn = UpdateImeData;

			//io.SetPlatformImeDataFn = IME::

			//io.GetClipboardTextFn = Clipboard::Get;
			//io.SetClipboardTextFn = Clipboard::Set;

			// ViewportにWindowHandle設定
			::ImGui::GetMainViewport()->PlatformHandleRaw = window.getHandle();

			// グラフィックリソース初期化
			InitDrawResource(renderPass, subpass);

			ImGuiStyle& style = ::ImGui::GetStyle();
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			style.GrabRounding = style.FrameRounding = 2.3f;
			style.WindowRounding = 6.0f;
			return true;
		}


		//@―---------------------------------------------------------------------------
		//! @brief      終了処理
		//@―---------------------------------------------------------------------------
		void Shutdown() {

			ImGuiIO& io = ::ImGui::GetIO();

			::ImGui::GetIO().Fonts->SetTexID(nullptr);

			if (auto data = GetObBackendData())delete data;
			if (auto data = GetObDrawData())delete data;
			io.BackendRendererUserData = nullptr;
			io.BackendPlatformUserData = nullptr;

		}


		//@―---------------------------------------------------------------------------
		//! @brief      フレーム開始
		//! @details	1フレームに1度実行
		//@―---------------------------------------------------------------------------
		void BeginFrame() {

			ImGuiIO& io = ::ImGui::GetIO();
			BackendData* bd = GetObBackendData();
			OB_ASSERT(bd != nullptr, "初期化されていません。ImGui_Init()の呼び出しを確認してください。");

			DrawData* drawData = GetObDrawData();
			OB_ASSERT(drawData != nullptr, "初期化されていません。ImGui_Init()の呼び出しを確認してください。");

			// 時間更新
			auto now = DateTime::Now();
			if (bd->time == DateTime()) {
				io.DeltaTime = 1.0f / 60.0f;
			} else {
				// TODO マイナス調査
				io.DeltaTime = abs(TimeSpan(now, bd->time).secondsF());
			}
			bd->time = now;
			bd->stringIndex = 0;

			// デバイス更新
			UpdateMouse();
			UpdateKeyboard();
			UpdateMouseCursor();

			// フォント画像が無ければ生成
			if (!drawData->fontTexture)
				CreateFontsTexture();

			// ディスプレイサイズ更新
			if (!bd->window) return;
			auto size = bd->window.getSize();
			io.DisplaySize = ImVec2(size.x, size.y);

			auto inputText = platform::Window::Main().getTextInput();
			if (inputText.empty()==false) {
				io.AddInputCharactersUTF8(inputText.c_str());
			}

			::ImGui::NewFrame();
		}


		//@―---------------------------------------------------------------------------
		//! @brief		描画コマンドを登録    
		//@―---------------------------------------------------------------------------
		void EndFrame(const Ref<rhi::CommandList>& commandList) {

			using namespace ob::rhi;

			::ImGui::Render();

			ImDrawData* draw_data = ::ImGui::GetDrawData();
			DrawData* bd = GetObDrawData();

			if (!draw_data || !bd)
				return;

			// ディスプレイサイズが0の場合はスキップ
			if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
				return;

			// バッファ生成
			if (!bd->vertexBuffer || bd->vertexCount < draw_data->TotalVtxCount)
			{
				bd->vertexCount = (u64)draw_data->TotalVtxCount + 5000;

				BufferDesc desc = BufferDesc::Vertex<ImDrawVert>(bd->vertexCount);
				desc.name = TC("::ImGui");
				bd->vertexBuffer = Buffer::Create(desc);

				if (bd->vertexBuffer)
					return;
			}
			if (!bd->indexBuffer || bd->indexCount < draw_data->TotalIdxCount)
			{
				bd->indexCount = (u64)draw_data->TotalIdxCount + 10000;

				BufferDesc desc = BufferDesc::Index<ImDrawIdx>(bd->indexCount);
				desc.name = TC("::ImGui");
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

			SetupRenderState(draw_data, commandList);

			int globalVertexOffset = 0;
			int globalIndexOffset = 0;

			for (int n = 0; n < draw_data->CmdListsCount; n++)
			{
				const ImDrawList* imCmdList = draw_data->CmdLists[n];

				for (int i = 0; i < imCmdList->CmdBuffer.Size; i++)
				{
					const ImDrawCmd* imCmdBuf = &imCmdList->CmdBuffer[i];

					// シザー/クリッピングの矩形をフレームバッファ空間に投影する
					ImVec2 clip = draw_data->DisplayPos;
					ImVec2 clip_min(imCmdBuf->ClipRect.x - clip.x, imCmdBuf->ClipRect.y - clip.y);
					ImVec2 clip_max(imCmdBuf->ClipRect.z - clip.x, imCmdBuf->ClipRect.w - clip.y);
					IntRect rect = { (s32)clip_min.x, (s32)clip_min.y, (s32)clip_max.x, (s32)clip_max.y };
					if (rect.width() <= 0 || rect.height() <= 0)
						continue;

					commandList->setScissorRect(&rect, 1);

					// テクスチャ設定
					SetDescriptorTableParam tables[] = {
						{*(Ref<DescriptorTable>*)imCmdBuf->TextureId,1}
					};
					commandList->setRootDesciptorTable(tables, std::size(tables));

					// 描画
					DrawIndexedParam param;
					param.indexCount = imCmdBuf->ElemCount;
					param.startIndex = imCmdBuf->IdxOffset + globalIndexOffset;
					param.startVertex = imCmdBuf->VtxOffset + globalVertexOffset;

					commandList->drawIndexed(param);
				}

				globalIndexOffset += imCmdList->IdxBuffer.Size;
				globalVertexOffset += imCmdList->VtxBuffer.Size;
			}

		}

		const char* ToImChars(StringView text,s32* out_len) {
			if (BackendData* bd = GetObBackendData()) {

				auto& tmp = bd->stringConvertBuffer;
				StringEncoder::Encode(text, tmp);
				auto len = tmp.size();
				if (bd->stringBuffer.size() < bd->stringIndex + len + 2) {
					return "バッファ不足";
				}
				auto ptr = bd->stringBuffer.data() + bd->stringIndex;

				if (out_len)*out_len = (s32)len;


				std::memcpy(ptr, tmp.data(), tmp.size()+1);

				*(ptr+len+2) = '\0';

				bd->stringIndex += len+2;

				return ptr;
			}
			return "初期化前";
		}

	}
}// namespcae ob

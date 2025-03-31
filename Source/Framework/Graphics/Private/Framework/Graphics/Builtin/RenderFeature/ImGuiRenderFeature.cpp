//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/RenderFeature/ImGuiRenderFeature.h>
#include <Framework/RHI/All.h>
#include <Framework/Input/All.h>

#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Plugins/ImGui/ImGuiRAII.h>
#ifdef OS_WINDOWS
#include <Windows.h>
#endif

#pragma region

//! @brief      ファイルオープン
ImFileHandle ImFileOpen(const char* filename, const char* mode) {
	using namespace ob::core;
	BitFlags<FileOpenMode> modes;
	if (strchr(mode, 'r'))modes.on(FileOpenMode::Read);
	if (strchr(mode, 'w'))modes.on(FileOpenMode::Write);
	if (strchr(mode, 'a'))modes.on(FileOpenMode::Append);
	if (strchr(mode, 'b') == nullptr)modes.on(FileOpenMode::Text);
	if (strchr(mode, '+'))LOG_FATAL("Not supported");// modes.on(FileOpenMode::Append);
	return new ob::core::File(filename, modes.get_enum());
}
//! @brief      ファイルクローズ
bool ImFileClose(ImFileHandle file) {
	if (file == nullptr) return false;
	delete file;
	return true;
}
//! @brief      ファイルサイズ取得
ob::u64 ImFileGetSize(ImFileHandle file) {
	return file ? file->size() : 0;
}
//! @brief      ファイル読み込み
ob::u64 ImFileRead(void* data, ob::u64 size, ob::u64 count, ImFileHandle file) {
	if (!file)return 0;
	if (file->canRead() == false)return 0;
	file->read(data, size * count);
	return size * count;
}
//! @brief      ファイル書き込み
ImU64 ImFileWrite(const void* data, ob::u64 size, ob::u64 count, ImFileHandle file) {
	if (!file)return 0;
	if (file->canWrite() == false)return 0;
	file->write(data, size * count);
	return size * count;
}

#pragma endregion

namespace ob::graphics {

	struct BackendData
	{
		String						clipboard;
	};

	//! @brief      バックエンドデータ取得
	static BackendData* GetBackendData()
	{
		return ::ImGui::GetCurrentContext() ? (BackendData*)::ImGui::GetIO().BackendPlatformUserData : nullptr;
	}

	//! @brief		スタイル設定
	static void SetupStyle() {
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 0.78f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 0.78f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.21f, 0.46f, 0.78f, 0.95f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		colors[ImGuiCol_Header] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
		colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		ImGuiStyle& style = ::ImGui::GetStyle();
		style.GrabRounding = style.FrameRounding = 2.3f;
		style.WindowRounding = 6.0f;
	}

	//! @brief      IME更新
	static void UpdateImeData(ImGuiViewport*, ImGuiPlatformImeData* data)
	{
		if (data->WantVisible) {
#ifdef OS_WINDOWS
			// TODO ウィンドウごとにIMEコンテキストが異なる
			HIMC hIMC = ImmGetContext((HWND)platform::Window::Main().getHandle());
			COMPOSITIONFORM d;
			d.dwStyle = CFS_POINT;
			d.ptCurrentPos.x = data->InputPos.x;
			d.ptCurrentPos.y = data->InputPos.y;
			ImmSetCompositionWindow(hIMC, &d);
#endif
		} else {

		}
	}

	//! @brief      クリップボードにテキストをコピー
	static void SetClipboardText(void*, const char* text)
	{
#ifdef OS_WINDOWS
		StringBase<char> text8 = text ? text : "";
		WString wtext;
		StringEncoder::Encode(text8, wtext);

		auto bufferSize = sizeof(wchar_t) * (wtext.size() + 1);

		if (auto hText = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, bufferSize)) {
			wchar_t* pText = (wchar_t*)(GlobalLock(hText));

			memcpy_s(pText, bufferSize, wtext.data(), bufferSize);
			if (hText) GlobalUnlock(hText);

			OpenClipboard(NULL);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hText);
			CloseClipboard();
		}
#endif
	}

	//! @brief      クリップボードからテキストを取得
	static const char* GetClipboardText(void*)
	{
		if (auto bd = GetBackendData()) {
#ifdef OS_WINDOWS
			if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {

				::OpenClipboard(NULL);

				HANDLE hData;
				hData = ::GetClipboardData(CF_UNICODETEXT);

				auto text = (wchar_t*)GlobalLock(hData);

				WString wtext = text ? text : L"";
				StringEncoder::Encode(wtext, bd->clipboard);

				GlobalUnlock(hData);

				::CloseClipboard();

			}
#endif
			return bd->clipboard.c_str();
		}
		return "Pasted";
	}

}

namespace ob::graphics {

	//! @brief		コンストラクタ
	ImGuiRenderFeature::ImGuiRenderFeature(RenderScene& scene)
	{
	}

	//! @brief		デストラクタ
	ImGuiRenderFeature::~ImGuiRenderFeature() {
	}

	//! @brief		タスクを追加
	void ImGuiRenderFeature::addTask(ImGuiHandle& handle, ImGuiDelegate func) {
		m_notifier.add(handle, func);
	}
	//! @brief		タスクを追加
	void ImGuiRenderFeature::AddTask(RenderScene& scene, ImGuiHandle& handle, ImGuiDelegate func) {
		if (auto feature = scene.findFeature<ImGuiRenderFeature>()) {
			feature->addTask(handle, func);
		}
	}
	void ImGuiRenderFeature::AddTask(RenderView& view, ImGuiHandle& handle, ImGuiDelegate func) {
		AddTask(view.getScene(), handle, func);
	}

	//! @brief		タスクを実行
	void ImGuiRenderFeature::executeTasks() {
		m_notifier.invoke();
	}



	//! @brief		コンストラクタ
	ImGuiRenderer::ImGuiRenderer(RenderView& view) 
		: m_view(view)
	{
		initializeContext();
		initializeResource();
	}

	//! @brief		デストラクタ
	ImGuiRenderer::~ImGuiRenderer() {
		ImGui::SetCurrentContext(m_imguiContext);
		ImPlot::SetCurrentContext(m_implotContext);

		auto data = reinterpret_cast<BackendData*>(::ImGui::GetIO().BackendPlatformUserData);
		SafeDelete(data);
		::ImGui::GetIO().BackendPlatformUserData = nullptr;

		ImPlot::DestroyContext(m_implotContext);
		ImGui::DestroyContext(m_imguiContext);
	}

	//! @brief		コンストラクタ
	bool ImGuiRenderer::render(FG& fg, FGTexture& target) {

		auto display = m_view.getDisplay();
		if (display == nullptr)return false;
		auto window = display->getDesc().window;


		// このビューのコンテキストを設定
		ImGui::SetCurrentContext(m_imguiContext);
		ImPlot::SetCurrentContext(m_implotContext);


		// 更新
		if (window) {
			ImGui::GetIO().DisplaySize = { std::max<f32>(window.getSize().x,1),std::max<f32>(window.getSize().y,1) };
			updateMouse(window);
			updateKeyboard(window);
		}
		updateTime();

		// 描画
		ImGui::NewFrame();

		if (auto feature = m_view.findFeature<ImGuiRenderFeature>()) {
			feature->executeTasks();
		}

		ImGui::EndFrame();
		ImGui::Render();

		updateBuffer();
		updateCommand();

		auto displaySize = ::ImGui::GetDrawData()->DisplaySize;


		struct ImGuiData {
			FGTexture target;
		};

		auto& data = fg.addPass<ImGuiData>(
			"ImGui",
			[&](FGBuilder& builder, ImGuiData& data) {
				data.target = builder.write(target);
			},
			[this, displaySize](const ImGuiData& data, FGResources& resources, rhi::CommandList& cmdList) {

				using namespace ob::rhi;

				auto texture = resources.get(data.target);
				Viewport vp(0, 0, texture->width(), texture->height(), 0, 1);

				cmdList.pushMarker("ImGui");

				cmdList.setRenderTarget(texture);
				cmdList.setPipelineState(m_pipeline);
				cmdList.setViewport(&vp, 1);
				cmdList.setVertexBuffer(m_vertexBuffer);
				cmdList.setIndexBuffer(m_indexBuffer);

				rhi::SetDescriptorTableParam params[] = {
					{m_constantTable,0}
				};
				cmdList.setRootDesciptorTable(params, std::size(params));


				for (auto& cmd : m_commands) {

					// テクスチャ設定
					SetDescriptorTableParam tables[] = {
						{*(Ref<DescriptorTable>*)cmd.texture,1}
					};

					cmdList.setScissorRect(&cmd.rect, 1);
					cmdList.setRootDesciptorTable(tables, std::size(tables));
					cmdList.drawIndexed(cmd.param);
				}

				cmdList.popMarker();
			}
		);

		target = data.target;
		return true;
	}

	//! @brief      コンテキストの初期化
	//! @details	ImGuiとImPlotのコンテキストを生成し、必要なオプションを設定する。
	void ImGuiRenderer::initializeContext() {

		m_imguiContext = ::ImGui::CreateContext();
		m_implotContext = ::ImPlot::CreateContext();

		ImGui::ScopedContext sc(m_imguiContext);

		BackendData* bd = nullptr;
		ImGuiIO& io = ::ImGui::GetIO();

		io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
		io.BackendFlags = ImGuiBackendFlags_HasMouseCursors;
		io.BackendPlatformUserData = bd = new BackendData();
		io.BackendPlatformName = "OctbitEngine";
		io.SetClipboardTextFn = SetClipboardText;
		io.GetClipboardTextFn = GetClipboardText;
		io.SetPlatformImeDataFn = UpdateImeData;

		// bd->window = window;
		SetupStyle();
	}

	//! @brief      リソースの初期化
	//! @details	RootSignatureやシェーダはRenderFeatureで共用することも可能。
	//!				実装をシンプルにするためView毎に生成しています。
	void ImGuiRenderer::initializeResource() {

		using namespace ob::rhi;

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
			desc.name = "ImGui";
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}

		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code =
				"SamplerState g_mainSampler:register(s0);						\n"
				"Texture2D g_mainTex:register(t0);								\n"
				"cbuffer vertexBuffer : register(b0) {							\n"
				"	float4x4 proj;												\n"
				"};																\n"
				"// IN / OUT													\n"
				"struct VsIn {													\n"
				"  float2 pos	:POSITION;										\n"
				"  float2 uv	    :TEXCOORD0;									\n"
				"  float4 color	:COLOR0;										\n"
				"};																\n"
				"struct PsIn {													\n"
				"  float4 pos	:SV_POSITION;									\n"
				"  float2 uv	    :TEXCOORD0;									\n"
				"  float4 color	:COLOR0;										\n"
				"};																\n"
				"// エントリ													\n"
				"struct PsOut {													\n"
				"  float4 color	:SV_TARGET0;									\n"
				"};																\n"
				"PsIn VS_Main(VsIn i) {											\n"
				"    PsIn o;													\n"
				"    o.pos = mul( proj, float4(i.pos.xy, 0, 1));				\n"
				"    o.color = i.color;								            \n"
				"    o.uv  = i.uv;				                                \n"
				"    return o;													\n"
				"}																\n"
				"PsOut PS_Main(PsIn i){											\n"
				"	PsOut o;													\n"
				"	o.color = g_mainTex.Sample(g_mainSampler,i.uv)*i.color;		\n"
				"    return o;											        \n"
				"}																\n";

			vs = Shader::CompileVS(code);
			ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(vs && ps);
		}

		{
			PipelineStateDesc desc;
			desc.name = "ImGui";
			desc.colors = { TextureFormat::RGBA8 };

			desc.rootSignature = signature;
			desc.vs = vs;
			desc.ps = ps;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,offsetof(ImDrawVert,pos),ElementType::Float,2),
				VertexAttribute(Semantic::TexCoord,offsetof(ImDrawVert,uv),ElementType::Float,2),
				VertexAttribute(Semantic::Color,offsetof(ImDrawVert,col),ElementType::UInt8,4),
			};
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;

			m_pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(m_pipeline);
		}

		initializeFont();
	}

	//! @brief      フォント画像生成
	void ImGuiRenderer::initializeFont()
	{
		ImGui::ScopedContext sc(m_imguiContext);
		ImGuiIO& io = ::ImGui::GetIO();
		unsigned char* pixels;

		// フォント読み込み
		// TODO ビューごとにリソースを生成する必要はないのでImGuiFeatureに共有リソースを登録しておく
		File file("Assets/Font/ipaexg.ttf");
		if (Blob blob(file); blob) {
			ImFontConfig config;
			m_fontBlod = ::ImGui::MemAlloc(blob.size());
			memcpy_s(m_fontBlod, blob.size(), blob.data(), blob.size());
			io.Fonts->AddFontFromMemoryTTF(m_fontBlod, blob.size(), 13.0f, &config, io.Fonts->GetGlyphRangesJapanese());
		}

		// テクスチャデータ生成
		int width, height, bytes_per_pixel;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

		Vector<IntColor> colors((u64)width * height);
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

		// グラフィックリソース生成
		m_fontTexture = rhi::Texture::Create("ImGuiFont", Size(width, height), colors);
		m_fontTextureTable = rhi::DescriptorTable::Create(rhi::DescriptorRangeType::SRV, 1);
		m_fontTextureTable->setResource(0, m_fontTexture);

		// システム登録
		io.Fonts->SetTexID((ImTextureID)&m_fontTextureTable);
	}

	//! @brief      マウス更新
	void ImGuiRenderer::updateMouse(const platform::Window& window)
	{
		using namespace ob::input;
		using namespace ob::platform;
		ImGuiIO& io = ::ImGui::GetIO();
		BackendData* bd = GetBackendData();

		if (bd == nullptr || !window)
			return;

		// ウィンドウフォーカス中の未処理
		const bool focused = window.getState().has(WindowState::Focused);
		if (focused)
		{
			// カーソル位置
			auto pos = window.getClientPoint(Mouse::GetPos());
			io.AddMousePosEvent((float)pos.x, (float)pos.y);

			// ボタン入力
			if (input::Mouse::Left.down())	io.AddMouseButtonEvent(0, true);
			if (input::Mouse::Left.up())	io.AddMouseButtonEvent(0, false);
			if (input::Mouse::Right.down())	io.AddMouseButtonEvent(1, true);
			if (input::Mouse::Right.up())	io.AddMouseButtonEvent(1, false);
			if (input::Mouse::Middle.down())io.AddMouseButtonEvent(2, true);
			if (input::Mouse::Middle.up())	io.AddMouseButtonEvent(2, false);

			// ホイール
			if (auto value = Mouse::WheelX.value(); value != 0.0f) io.AddMouseWheelEvent(value, 0);
			if (auto value = Mouse::Wheel.value(); value != 0.0f) io.AddMouseWheelEvent(0, value * 0.01);

		}
	}

	//! @brief      キーボード更新
	void ImGuiRenderer::updateKeyboard(platform::Window& window) {

		using namespace ob::input;

		// NOTE 同時押し対応が必要？
		struct KeyMap { Key from; ImGuiKey to; };
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

		ImGuiIO& io = ::ImGui::GetIO();

		for (auto& [from, to] : keyMap) {
			if (Keyboard::GetButton(from).down())io.AddKeyEvent(to, true);
			if (Keyboard::GetButton(from).up())io.AddKeyEvent(to, false);
		}

		auto inputText = window.getTextInput();
		io.AddInputCharactersUTF8(inputText.c_str());

		if (auto bd = GetBackendData()) {
			bd->clipboard.clear();
		}

	}

	//! @brief      時間更新
	void ImGuiRenderer::updateTime()
	{
		auto& io = ::ImGui::GetIO();
		auto now = DateTime::Now();
		if (m_time == DateTime()) {
			// 初回更新
			io.DeltaTime = 1.0f / 60.0f;
		} else {
			io.DeltaTime = std::max(std::abs(TimeSpan(now, m_time).secondsF()), 1.0f / 10000.0f);
		}

		m_time = now;
	}

	//! @brief		バッファ更新
	void ImGuiRenderer::updateBuffer() {

		using namespace ob::rhi;

		const auto overAllocVertexSize = 5000;
		const auto overAllocIndexSize = 5000;

		ImDrawData* draw_data = ::ImGui::GetDrawData();

		// ビューポートバッファ更新
		if (!m_constantBuffer)
		{
			auto newSize = 512;

			BufferDesc desc = BufferDesc::Vertex<ImDrawVert>(newSize);
			desc.name = "ImGuiConstant";
			m_constantBuffer = Buffer::Create(desc);

			m_constantTable = DescriptorTable::Create(DescriptorRangeType::CBV, 1);
			m_constantTable->setResource(0, m_constantBuffer);
		}
		{
			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
			auto mvp = Matrix::Orthographic(L, R, B, T, 0, 1);
			m_constantBuffer->updateDirect(mvp, 0);
		}

		// 頂点バッファ生成
		if (!m_vertexBuffer || m_vertexCount < draw_data->TotalVtxCount)
		{
			auto newSize = (u64)draw_data->TotalVtxCount + overAllocVertexSize;

			BufferDesc desc = BufferDesc::Vertex<ImDrawVert>(newSize);
			desc.name = "ImGuiVertex";
			m_vertexBuffer = Buffer::Create(desc);
			m_vertexCount = newSize;
		}

		// インデックスバッファ生成
		if (!m_indexBuffer || m_indexCount < draw_data->TotalIdxCount)
		{
			auto newSize = (u64)draw_data->TotalIdxCount + overAllocIndexSize;

			BufferDesc desc = BufferDesc::Index<ImDrawIdx>(newSize);
			desc.name = "ImGuiIndex";
			m_indexBuffer = Buffer::Create(desc);
			m_indexCount = newSize;
		}

		// バッファアップロード
		if (m_vertexBuffer && m_indexBuffer) {
			m_vertexBuffer->updateDirect(
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
			m_indexBuffer->updateDirect(
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
		}
	}

	//! @brief		バッファ更新
	void ImGuiRenderer::updateCommand() {

		// コマンド更新
		m_commands.clear();

		ImDrawData* drawData = ::ImGui::GetDrawData();

		size_t globalIndexOffset = 0;
		size_t globalVertexOffset = 0;

		for (auto& cmdList : drawData->CmdLists) {
			for (auto& cmdBuf : cmdList->CmdBuffer) {

				// シザー/クリッピングの矩形をフレームバッファ空間に投影する
				ImVec2 clip = drawData->DisplayPos;
				ImVec2 clip_min(cmdBuf.ClipRect.x - clip.x, cmdBuf.ClipRect.y - clip.y);
				ImVec2 clip_max(cmdBuf.ClipRect.z - clip.x, cmdBuf.ClipRect.w - clip.y);

				DrawCommand cmd;
				cmd.rect = { (s32)clip_min.x, (s32)clip_min.y, (s32)clip_max.x, (s32)clip_max.y };
				cmd.texture = cmdBuf.TextureId;
				cmd.param.indexCount = cmdBuf.ElemCount;
				cmd.param.startIndex = cmdBuf.IdxOffset + globalIndexOffset;
				cmd.param.startVertex = cmdBuf.VtxOffset + globalVertexOffset;

				if (cmd.rect.empty()) continue;
				m_commands.push_back(cmd);

			}
			globalIndexOffset += cmdList->IdxBuffer.Size;
			globalVertexOffset += cmdList->VtxBuffer.Size;
		}

	}


}
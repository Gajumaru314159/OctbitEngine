//***********************************************************
//! @file
//! @brief		ディスプレイ・チェーン実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DirectX12Display.h"
#include <Framework/RHI/DescriptorLayout.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Shader.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Plugins/DirectX12RHI/Command/DirectX12CommandList.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <magic_enum.hpp>
namespace {
	int static const s_maxDisplayCount = 4;
}

namespace ob::rhi::dx12 {

	//! @brief  コンストラクタ
	DirectX12Display::DirectX12Display(DirectX12RHI& rDevice, const DisplayDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{

		if (!desc.window.isValid()) {
			LOG_ERROR_EX("Graphic", "Windowが設定されていません。");
			return;
		}

		// サイズが指定されていない場合はウィンドウサイズを使用
		if (m_desc.size.width == 0 || m_desc.size.height == 0) {
			auto size = m_desc.window.getSize();
			m_desc.size = Size(size.x, size.y);
		}
		m_syncInterval = desc.vsync ? 1 : 0;
		m_flags = 0;// desc.vsync ? 0 : (DXGI_PRESENT_ALLOW_TEARING | DXGI_PRESENT_DO_NOT_WAIT);

		if (!createDisplay(rDevice))return;
		if (!createResources(rDevice))return;
		if (!createBuffers(rDevice))return;

		m_desc.window.addEventListener(m_hEvent, { *this,&DirectX12Display::onWindowChanged });

		manage();
	}


	//! @brief      名前を取得
	const String& DirectX12Display::getName()const {
		return m_desc.name;
	}


	//! @brief  スワップチェーン生成
	bool DirectX12Display::createDisplay(DirectX12RHI& rDevice) {
		auto& window = m_desc.window;

		BOOL allowTearing = false;
		UINT sampleQuarity = 0;
		UINT sampleCount = 1;
		HWND hWnd = (HWND)window.getHandle();
		{
			{
				D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS feature{};
				auto result = rDevice.getNative()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &feature, sizeof(feature));
				if (SUCCEEDED(result)) {
					LOG_INFO_EX("Graphic", "最大マルチサンプルカウント={}", feature.SampleCount);
					LOG_INFO_EX("Graphic", "最大マルチサンプルクオリティ={}", feature.NumQualityLevels);
					//sampleQuarity = feature.SampleCount;
					//sampleCount = feature.NumQualityLevels;
				}
			}

			rDevice.getFactory()->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = m_desc.size.width;                                 // 画面解像度【横】
		swapChainDesc.BufferDesc.Height = m_desc.size.height;                               // 画面解像度【縦】
		swapChainDesc.BufferDesc.Format = TypeConverter::Convert(m_desc.format);            // ピクセルフォーマット
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;                             // リフレッシュ・レート分子
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 00;                            // リフレッシュ・レート分母
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // スキャンラインの順番 => 指定なし
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;                     //解像度に合うように同補正するか => 拡大

		swapChainDesc.SampleDesc.Quality = sampleQuarity;                                   // マルチサンプル・クオリティ
		swapChainDesc.SampleDesc.Count = sampleCount;                                       // マルチサンプル・カウント

		swapChainDesc.BufferCount = m_desc.bufferCount;						                // バッファの数
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			            // バックバッファとして使用
		swapChainDesc.OutputWindow = hWnd;                                                  // ウィンドウ
		swapChainDesc.Windowed = TRUE;                                                      // ※公式リファレンスによるとフルスクリーン指定は別ので行う
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                           // Present後破棄

		swapChainDesc.Flags =
			// (allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0) |
			//DXGI_SWAP_CHAIN_FLAG_NONPREROTATED |                  // フルスクリーン時自動回転
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH |                // ResizeTargetでサイズ変更許可
			//DXGI_SWAP_CHAIN_FLAG_DISPLAY_ONLY |                   // リモートアクセス禁止
			//DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT |  // フルスクリーン以外で描画待機
			//DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO |               // フルスクリーンビデオ
			//DXGI_SWAP_CHAIN_FLAG_YUV_VIDEO |                      // YUVビデオのスワップチェーン
			0;

		// スワップチェイン生成
		auto result = rDevice.getFactory()->CreateSwapChain(
			rDevice.getCommandQueue().Get(),
			&swapChainDesc,
			(IDXGISwapChain**)m_swapChain.ReleaseAndGetAddressOf());

		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "IDXGIFactory::CreateSwapChain()");
			return false;
		}


		if (allowTearing)
		{
			// When tearing support is enabled we will handle ALT+Enter key presses in the
			// window message loop rather than let DXGI handle it by calling SetFullscreenState.
			//rDevice.getFactory()->MakeWindowAssociation((HWND)m_desc.window.getHandle(), DXGI_MWA_NO_ALT_ENTER);
		}


		if (window.isMainWindow()) {

			// Alt + Enter でウィンドウモードに変わらないようにする 
			rDevice.getFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

			if (window.getMode() == platform::WindowMode::FullScreen) {
				// TODO フルスクリーンの場合バックバッファをリサイズ
			}
		}

		return true;
	}


	//! @brief      レンダーテクスチャを初期化
	bool DirectX12Display::createBuffers(DirectX12RHI& rDevice) {

		if (!is_in_range(m_desc.bufferCount, 1, s_maxDisplayCount)) {
			LOG_ERROR_EX("Graphic", "バックバッファの枚数が不正です。[Min=1,Max={0},Value={1}]", s_maxDisplayCount, m_desc.bufferCount);
			return false;
		}

		HRESULT result;

		// バッファを生成
		m_textures.clear();
		m_textures.reserve(m_desc.bufferCount);

		// レンダーターゲットビュー生成
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = TypeConverter::Convert(m_desc.format);
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		for (s32 i = 0; i < m_desc.bufferCount; ++i) {

			ComPtr<ID3D12Resource> resource;
			result = m_swapChain->GetBuffer(i, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				// 生成が正しければ呼ばれないはず
				Utility::OutputFatalLog(result, "IDXGIDisplay::GetBuffer()");
				return false;
			}

			auto name = Format("{}_{}", m_desc.name, i);

			auto& texture = m_textures.emplace_back(new DirectX12Texture(rDevice, resource, D3D12_RESOURCE_STATE_PRESENT, name));

			m_viewport = CD3DX12_VIEWPORT(resource.Get());
			m_scissorRect = CD3DX12_RECT(0, 0, (UINT)m_viewport.Width, (UINT)m_viewport.Height);
		}

		return true;
	}


	//! @brief  コンストラクタ
	bool DirectX12Display::createResources(DirectX12RHI& rDevice) {

		{
			Vec2 vertices[] = {
				{-1,-1},
				{+1,-1},
				{-1,+1},
				{+1,-1},
				{+1,+1},
				{-1,+1},
			};
			BufferDesc bdesc = BufferDesc::Vertex<Vec2>(std::size(vertices));
			bdesc.name = m_desc.name + "_DisplayVertices";
			m_verices = Buffer::Create(bdesc);
			m_verices->updateDirect(bdesc.size, vertices);
		}

		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code;
			code.append("SamplerState g_mainSampler:register(s0);						\n");
			code.append("Texture2D g_mainTex:register(t0);								\n");
			code.append("// IN / OUT														\n");
			code.append("struct VsIn {													\n");
			code.append("  float2 pos	:POSITION;										\n");
			code.append("};																\n");
			code.append("struct PsIn {													\n");
			code.append("  float4 pos	:SV_POSITION;									\n");
			code.append("  float2 uv	    :TEXCOORD;									    \n");
			code.append("};																\n");
			code.append("// エントリ														\n");
			code.append("PsIn VS_Main(VsIn i) {											\n");
			code.append("    PsIn o;														\n");
			code.append("    o.pos = float4(i.pos*float2(2,-2)-1,0,1);				    \n");
			code.append("    o.uv = i.pos.xy;								            \n");
			code.append("    return o;													\n");
			code.append("}																\n");
			code.append("float4 PS_Main(PsIn i):SV_TARGET0{								\n");
			code.append("    return g_mainTex.Sample(g_mainSampler,i.uv);		        \n");
			code.append("}																\n");

			vs = Shader::CompileVS(code);
			ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(vs && ps);
		}

		m_layout = DescriptorLayout::Create({ Binding::Texture(0) });

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc;
			desc.layouts = { m_layout };
			desc.samplers = { StaticSamplerDesc(SamplerDesc(),0) };
			desc.name = m_desc.name;
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}

		Ref<PipelineState> pipeline;
		{
			PipelineStateDesc desc;
			desc.name = m_desc.name;
			desc.colors = { m_desc.format };

			desc.rootSignature = signature;
			desc.vs = vs;
			desc.ps = ps;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,0,ElementType::Float,2),
			};
			desc.vertexLayout.vertexStride = sizeof(Vec2);
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
			desc.depthStencil.stencil.enable = false;

			pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(pipeline);
		}

		m_signature = signature;
		m_pipeline = pipeline;

		return true;
	}


	//! @brief      カラースペースを設定
	bool DirectX12Display::setColorSpace() {
		bool isHdrEnabled = m_desc.hdr;
		if (!isHdrEnabled)return false;

		// TODO Rec2020以外の指定対応
		DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
		UINT colorSpaceSupport;

		auto result = m_swapChain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport);
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "IDXGIDisplay::CheckColorSpaceSupport()");
			return false;
		}

		if (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) {
			result = m_swapChain->SetColorSpace1(colorSpace);
			if (FAILED(result)) {
				Utility::OutputFatalLog(result, "IDXGIDisplay::SetColorSpace1()");
				return false;
			}
		}
		return true;
	}


	//! @brief  デストラクタ
	DirectX12Display::~DirectX12Display() {

	}


	//! @brief  妥当なオブジェクトか
	bool DirectX12Display::isValid()const {
		return !m_textures.empty();
	}


	//! @brief  定義を取得
	const DisplayDesc& DirectX12Display::getDesc()const noexcept {
		return m_desc;
	}


	//! @brief      更新
	//! 
	//! @details    表示するテクスチャを次のバックバッファにします。
	void DirectX12Display::update() {

		if (!m_desc.window.isValid())return;
		if (!m_visible)return;

		auto result = m_swapChain->Present(m_syncInterval, m_flags);

		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "IDXGUIDisplay::Present()");
			LOG_FATAL_EX("Graphic", "スワップチェーンの更新に失敗")
				return;
		}

		auto index = m_swapChain->GetCurrentBackBufferIndex();
		m_textures.setIndex(index);

	}


	//! @brief      イベントリスナ追加
	void DirectX12Display::addEventListener(DisplayEventHandle& handle, DisplayEventDelegate func) {
		m_notifier.add(handle, func);
	}



	//! @brief      デスクリプタCPUハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Display::getCpuHandle()const {
		return m_textures.current().cast<DirectX12Texture>()->getRTV().getCpuHandle();
	}


	//! @brief      デスクリプタGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE DirectX12Display::getGpuHandle()const {
		return m_textures.current().cast<DirectX12Texture>()->getRTV().getGpuHandle();
	}


	//! @brief      ビューポートを取得
	D3D12_VIEWPORT DirectX12Display::getViewport()const {
		return m_viewport;
	}


	//! @brief      シザー矩形を取得
	D3D12_RECT DirectX12Display::getScissorRect()const {
		return m_scissorRect;
	}


	//! @brief      リソース取得
	ID3D12Resource* DirectX12Display::getResource()const {

		return m_textures.current().cast<DirectX12Texture>()->getResource();
	}


	//! @brief      バッファへコピー
	void DirectX12Display::recordApplyDisplay(DirectX12CommandList& cmdList, const Ref<Texture>& texture) {

		// テクスチャが違う場合再バインド
		if (m_bindedTexture != texture) {

			m_bindedTextureTable.reset();
			m_bindedTexture = texture;

			if (m_bindedTexture) {
				m_bindedTextureTable = DescriptorTable::Create({ m_layout });
				m_bindedTextureTable->setResource(0, m_bindedTexture);
			}

		}

		// バインドされていなければスキップ
		if (!m_bindedTextureTable)
			return;

		{
			cmdList.pushMarker("Apply Display");



			RenderPassDesc renderPass;
			renderPass.colors.emplace_back(m_textures.current(), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

			cmdList.beginRenderPass(renderPass);

			cmdList.setPipelineState(m_pipeline);

			SetDescriptorTableParam tableParam(m_bindedTextureTable, 0);
			cmdList.setRootDesciptorTable(&tableParam, 1);

			cmdList.setVertexBuffer(m_verices);

			DrawParam drawParam;
			drawParam.startVertex = 0;
			drawParam.vertexCount = 6;
			cmdList.draw(drawParam);

			cmdList.endRenderPass();

			// Present準備
			if (auto texture = m_textures.current().cast<DirectX12Texture>()) {

				D3D12_RESOURCE_BARRIER barrier;
				if (texture->addResourceTransition(barrier, D3D12_RESOURCE_STATE_PRESENT)) {
					cmdList.getNative()->ResourceBarrier(1, &barrier);
				}

			}

			cmdList.popMarker();
		}

	}


	//! @brief      ウィンドウの更新イベント
	void DirectX12Display::onWindowChanged(const platform::WindowEventArgs& args) {

		if (args.type == platform::WindowEventType::Size || args.type == platform::WindowEventType::Maximize) {
			if (!args.isSizing) {

				DXGI_SWAP_CHAIN_DESC desc = {};
				m_swapChain->GetDesc(&desc);

				if (desc.BufferDesc.Width == args.newSize.x && desc.BufferDesc.Height == args.newSize.y)
					return;

				m_newSize = args.newSize;

				m_resizeCountDown = 2;

				m_device.clearCommands();


				m_desc.size.width = (s32)args.newSize.x;
				m_desc.size.height = (s32)args.newSize.y;

				for (s32 i = 0; i < m_desc.bufferCount; ++i) {
					// リソースが使用中だとResizeBuffersに失敗する。
					// TODO 無効なメモリを描画に使用しそうだがいったん保留
					m_textures.at(i).cast<DirectX12Texture>()->releaseNative();
				}

				// リサイズ
				auto result = m_swapChain->ResizeBuffers(m_desc.bufferCount, 0, 0, desc.BufferDesc.Format, desc.Flags);
				if (FAILED(result)) {
					Utility::OutputErrorLog(result, "IDXGISwapChain::ResizeBuffersに失敗");
					return;
				}

				createBuffers(m_device);

				LOG_TRACE("ディスプレイをリサイズ ({},{}) -> ({},{})", desc.BufferDesc.Width, desc.BufferDesc.Height, m_desc.size.width, m_desc.size.height);

				m_notifier.invoke();

			}
		}

		if (args.type == platform::WindowEventType::Minimize) {
			m_visible = false;
		}
		if (args.type == platform::WindowEventType::Maximize || args.type == platform::WindowEventType::Move) {
			m_visible = true;
		}

		auto a = magic_enum::enum_name(args.type);

	}

}
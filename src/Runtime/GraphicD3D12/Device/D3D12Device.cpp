//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "D3D12Device.h"

namespace ob {
    namespace graphic {

        Device::Device() {
			Initialize();
        }



        void Device::Initialize() {
			//======================================//
			// ウィンドウの生成
			//======================================//

			if (FAILED(CreateMainWindow())) {
				LOG_ERROR_EX(TEXT("Graphic"),TEXT("Failed CreateMainWindow()"));
			}

			//======================================//
			// DirectXの初期化
			//======================================//

			// デバイスの初期化
			if (FAILED(InitializeDXGIDevice())) {
				LOG_ERROR_EX(TEXT("Graphic"), TEXT("Failed InitializeDXGIDevice()"));
			}
			// コマンドキューの初期化
			if (FAILED(InitializeCommand())) {
				LOG_ERROR_EX(TEXT("Graphic"), TEXT("Failed InitializeCommand()"));
			}
			// スワップチェーンの作成
			if (FAILED(CreateSwapChain(m_hwnd))) {
				LOG_ERROR_EX(TEXT("Graphic"), TEXT("Failed CreateSwapChain()"));
			}
			// 最終レンダリング先の作成
			if (FAILED(CreateFinalRenderTargets())) {
				LOG_ERROR_EX(TEXT("Graphic"), TEXT("Failed CreateFinalRenderTargets()"));
			}
			//フェンスの作成
			if (FAILED(CreateFence())) {
				LOG_ERROR_EX(TEXT("Graphic"), TEXT("Failed CreateFence()"));
			}

			ShowWindow(m_hwnd, SW_SHOW);

			// デフォルトアセットの作成
			//if (CreateDefaultAssets() == -1) {
			//	return -1;
			//}

			//Test();
			//return 0;
        }


		s32 Device::SwapScreen(SPtr<IRenderTexture>& renderTarget) {
			// レンダー結果をバックバッファに書き込み
			static void** preRenderTarget = nullptr;
			if (preRenderTarget != (void**)renderTarget.get()) {
				m_material->SetTexture(TEXT("tex"), renderTarget);
			}

			auto pipelinePtr = reinterpret_cast<GraphicPipeline*>(m_graphicPipeline.get());
			pipelinePtr->SetGraphicPipeline(m_cmdList);

			auto materialPtr = reinterpret_cast<Material*>(m_material.get());
			materialPtr->SetMaterial(m_cmdList);

			auto shapePtr = reinterpret_cast<Shape*>(m_shape.get());
			shapePtr->Draw(m_cmdList, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



			// スワップのためにリソースバリアを設定
			auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();
			m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_backBuffers[bbIdx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));


			m_cmdList->Close();


			// コマンドリストの実行
			static ArrayList< ID3D12CommandList*> cmdlists;
			for (auto rt : ms_renderTextureQueue) {
				RenderTexture* ptr = reinterpret_cast<RenderTexture*>(rt);
				cmdlists.push_back(ptr->GetCommandList());
			}
			cmdlists.push_back(m_cmdList.Get());
			m_cmdQueue->ExecuteCommandLists((UINT)cmdlists.size(), cmdlists.data());
			cmdlists.clear();


			// 描画待ち
			m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
			if (m_fence->GetCompletedValue() < m_fenceVal) {
				auto event = CreateEvent(nullptr, false, false, nullptr);
				m_fence->SetEventOnCompletion(m_fenceVal, event);
				WaitForSingleObject(event, INFINITE);
				CloseHandle(event);
			}


			m_cmdAllocator->Reset();
			m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
			for (auto rt : ms_renderTextureQueue) {
				auto ptr = reinterpret_cast<RenderTexture*>(rt);
				ptr->ResetCommand();
			}
			ms_renderTextureQueue.clear();





			m_swapchain->Present(1, 0);

			MSG msg = {};
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//もうアプリケーションが終わるって時にmessageがWM_QUITになる
			if (msg.message == WM_QUIT) {
				return -1;
			}

			//DirectX処理
			//バックバッファのインデックスを取得
			bbIdx = m_swapchain->GetCurrentBackBufferIndex();

			m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_backBuffers[bbIdx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

			//レンダーターゲットを指定
			auto rtvH = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
			rtvH.ptr += (SIZE_T)bbIdx * ms_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * bbIdx;

			m_cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

			//画面クリア
			float clearColor[] = { 0,0,0,1 };
			m_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

			//ビューポート、シザー矩形のセット
			m_cmdList->RSSetViewports(1, &m_viewport);
			m_cmdList->RSSetScissorRects(1, &m_scissorrect);


			m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			return 0;
		}

		// ここからprivate関数

		HRESULT Device::CreateMainWindow() {
			// ウィンドウクラス生成＆登録
			WNDCLASSEX w = {};
			w.cbSize = sizeof(WNDCLASSEX);
			w.lpfnWndProc = (WNDPROC)WindowProcedure;
			w.lpszClassName = L"DX12Sample";
			w.hInstance = GetModuleHandle(nullptr);

			RegisterClassEx(&w);

			// ウィンドウサイズ
			m_windowWidth = 1280;
			m_windowHeight = 720;
			RECT wrc = { 0, 0, m_windowWidth, m_windowHeight };

			AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

			// ウィンドウオブジェクトの生成
			m_hwnd = CreateWindow(
				w.lpszClassName,       // クラス名
				L"DX12テスト",    // ウィンドウタイトル
				WS_OVERLAPPEDWINDOW,   // ウィンドウスタイル
				CW_USEDEFAULT,         // X
				CW_USEDEFAULT,         // Y
				wrc.right - wrc.left,  // W
				wrc.bottom - wrc.top,  // H
				nullptr,               // 親ウィンドウ
				nullptr,               // メニューハンドル
				w.hInstance,           // 追加パラメータ
				nullptr);
			return S_OK;
		}

		HRESULT Device::InitializeDXGIDevice() {
			UINT flagsDXGI = 0;
			flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
			auto result = CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				return result;
			}

			// アダプターの列挙し、メモリ量が最大のグラフィックボードを選択
			std::vector<IDXGIAdapter*> adapters;

			IDXGIAdapter* tmpAdapter = nullptr;
			IDXGIAdapter* selectedAdapter = nullptr;
			SIZE_T maxVideoCardMemory = 0;
			for (int i = 0; m_dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
				DXGI_ADAPTER_DESC adesc = {};
				tmpAdapter->GetDesc(&adesc);

				SIZE_T videoCardMemory = (SIZE_T)(adesc.DedicatedVideoMemory / 1024 / 1024);  //(MB)

				if (maxVideoCardMemory < videoCardMemory) {
					selectedAdapter = tmpAdapter;
					maxVideoCardMemory = videoCardMemory;
				}
			}

			// 対応しているレベルで生成
			D3D_FEATURE_LEVEL featureLevel;
			D3D_FEATURE_LEVEL levels[] = {
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0,
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
			};

			result = S_FALSE;
			for (auto level : levels) {
				if (SUCCEEDED(D3D12CreateDevice(selectedAdapter, level, IID_PPV_ARGS(&ms_device)))) {
					featureLevel = level;
					result = S_OK;
					break;
				}
			}
			return result;
		}

		HRESULT Device::InitializeCommand() {
			auto result = ms_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				return result;
			}
			result = ms_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				return result;
			}


			D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
			cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;       // コマンドリストと合わせる
			cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;        // タイムアウトなし
			cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  // コマンドキューの優先度
			cmdQueueDesc.NodeMask = 0;                                    // GPUが1つの時は0、複数の時は識別用のbitを指定

			result = ms_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_cmdQueue.ReleaseAndGetAddressOf()));  //コマンドキュー生成

			return result;
		}

		HRESULT Device::CreateSwapChain(const HWND& hwnd) {
			RECT rc = {};
			GetWindowRect(hwnd, &rc);

			DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
			swapchainDesc.Width = rc.right - rc.left;                      // 画面解像度【横】
			swapchainDesc.Height = rc.bottom - rc.top;                      // 画面解像度【縦】
			swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;              // ピクセルフォーマット
			swapchainDesc.Stereo = false;                                   // 3Dディスプレイかどうか
			swapchainDesc.SampleDesc.Count = 1;                                       // マルチサンプル指定
			swapchainDesc.SampleDesc.Quality = 0;                                       // マルチサンプル指定
			swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;                  // CPUのバックバッファへのアクセス方法
			swapchainDesc.BufferCount = 2;                                       // バッファの数。ダブルバッファのため2。
			swapchainDesc.Scaling = DXGI_SCALING_STRETCH;                    // リサイズ時の挙動。見た目に合わせる。
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;           // スワップ後に速やかに破棄
			swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;             // バックバッファの透過の挙動
			swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // ウィンドウ⇔フルスクリーン切り替え可能

			auto result = m_dxgiFactory->CreateSwapChainForHwnd(
				m_cmdQueue.Get(),
				hwnd,
				&swapchainDesc,
				nullptr,  // フルスクリーン時の設定
				nullptr,  // アウトプット
				(IDXGISwapChain1**)m_swapchain.ReleaseAndGetAddressOf());
			return result;
		}

		HRESULT Device::CreateFinalRenderTargets() {
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;   // レンダーターゲットビュー
			heapDesc.NodeMask = 0;                                // GPUが1つの時は0、複数の時は識別用のbitを指定
			heapDesc.NumDescriptors = 2;                                // ディスクリプタの数。表と裏バッファの２つ。
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // ビューの情報をシェーダから参照する必要があるか

			auto result = ms_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_rtvHeaps.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				return result;
			}


			DXGI_SWAP_CHAIN_DESC swcDesc = {};
			result = m_swapchain->GetDesc(&swcDesc);
			m_backBuffers.resize(swcDesc.BufferCount);
			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();

			//SRGBレンダーターゲットビュー設定
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;


			for (U32 i = 0; i < swcDesc.BufferCount; ++i) {
				result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(m_backBuffers[i].ReleaseAndGetAddressOf()));
				ms_device->CreateRenderTargetView(m_backBuffers[i].Get(), &rtvDesc, handle);
				handle.ptr += ms_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			}



			m_viewport = CD3DX12_VIEWPORT(m_backBuffers[0].Get());
			m_scissorrect = CD3DX12_RECT(0, 0, (U32)m_viewport.Width, (U32)m_viewport.Height);

			return result;
		}

		HRESULT Device::CreateFence() {
			m_fenceVal = 0;
			return ms_device->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		}

		s32 Device::CreateDefaultAssets() {
			char cdir[255];
			GetCurrentDirectoryA(255, cdir);

			//DefaultAsset::Instance()->whiteTex = MSPtr<Texture>(m_Dev, 4, 4, ConvertTextureFormat(TextureFormat::RGBA8));
			DefaultAsset::Instance()->whiteTex = MSPtr<ImageTexture>(Path(TC("white.png")));
			if (!DefaultAsset::Instance()->whiteTex->IsValid())return -1;

			{
				String errorDest;

				String vssrc;
				vssrc.append("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};");
				vssrc.append("\nOutput VSMain(float4 pos : POSITION ,float2 uv : TEXCOORD) {");
				vssrc.append("\n    Output o;");
				vssrc.append("\n    o.pos = pos;");
				vssrc.append("\n    o.uv = uv;");
				vssrc.append("\n    return o;");
				vssrc.append("\n}");
				String pssrc;
				pssrc.append("\nTexture2D tex:register(t0);");
				pssrc.append("\nSamplerState smp:register(s0);");
				pssrc.append("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};");
				pssrc.append("\nfloat4 PSMain(Output i) : SV_TARGET{");
				pssrc.append("\n    return float4(tex.Sample(smp,i.uv));");
				pssrc.append("\n}");

				auto vs = CreateShader(vssrc, TC("VSMain"), ShaderType::VERTEX, errorDest);
				auto ps = CreateShader(pssrc, TC("PSMain"), ShaderType::PIXEL, errorDest);
				if (vs == nullptr || ps == nullptr) {
					return -1;
				}

				// 描画用パイプラインの作成
				GraphicPipelineDesc pipeline;
				pipeline.vs = vs;
				pipeline.ps = ps;
				pipeline.numRenderTargets = 1; pipeline.cullMode = CullMode::NONE;
				pipeline.zTest = false;
				pipeline.zWrite = true;
				pipeline.useStencil = false;

				m_graphicPipeline = CreateGraphicPipeline(pipeline);
				if (m_graphicPipeline == nullptr) {
					return -1;
				}


				m_material = CreateMaterial(m_graphicPipeline, -1, -1);


				// 描画用シェイプの作成
				m_shape = CreateShape();
				float m_Vertices[4 * 5] = {
					-1.0f,1.0f,0.0f,0.0f,0.0f,
					1.0f,1.0f,0.0f,1.0f,0.0f,
					-1.0f,-1.0f,0.0f,0.0f,1.0f,
					1.0f,-1.0f,0.0f,1.0f,1.0f
				};
				m_shape->SetVertexData((Byte*)m_Vertices, sizeof(F32) * 5, 4);

				ArrayList<U32> indices = { 0,1,2,2,1,3 };
				m_shape->SetIndices(indices);

				if (m_shape == nullptr) {
					return -1;
				}
			}
			return 0;
		}
    }
    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

}// namespace ob
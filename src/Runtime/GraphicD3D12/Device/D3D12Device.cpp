//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "D3D12Device.h"
#include "../Swapchain/D3D12Swaphain.h"
#include "../CommandList/D3D12CommandList.h"

namespace ob {
    namespace graphic {

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
        D3D12Device::D3D12Device(){
			Initialize();
        }


		//@―---------------------------------------------------------------------------
		//! @brief  スワップチェーンの生成
		//@―---------------------------------------------------------------------------
		Ref<Swapchain> D3D12Device::createSwapchainImpl(const SwapchainDesc& desc) {
			return new D3D12Swapchain(m_dxgiFactory,m_commandQueue,desc);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  コマンドリストの生成
		//@―---------------------------------------------------------------------------
		Ref<CommandList> D3D12Device::createCommandListImpl(CommandListType type) {
			return MakeRef<D3D12CommandList>(m_device, type);
		}


		//@―---------------------------------------------------------------------------
		//! @brief  初期化
		//@―---------------------------------------------------------------------------
        void D3D12Device::Initialize() {

			// デバイスの初期化
			if (FAILED(InitializeDXGIDevice())) {
				LOG_ERROR_EX("Graphic", "Failed InitializeDXGIDevice()");
			}
			
			// コマンドキューの初期化
			if (FAILED(InitializeCommand())) {
				LOG_ERROR_EX("Graphic", "Failed InitializeCommand()");
			}

			//// スワップチェーンの作成
			//if (FAILED(CreateSwapChain())) {
			//	LOG_ERROR_EX("Graphic", "Failed CreateSwapChain()");
			//}
			//// 最終レンダリング先の作成
			//if (FAILED(CreateFinalRenderTargets())) {
			//	LOG_ERROR_EX("Graphic", "Failed CreateFinalRenderTargets()");
			//}
			////フェンスの作成
			//if (FAILED(CreateFence())) {
			//	LOG_ERROR_EX("Graphic", "Failed CreateFence()");
			//}

        }


		//@―---------------------------------------------------------------------------
		//! @brief  DXGIDeviceの初期化
		//@―---------------------------------------------------------------------------
		HRESULT D3D12Device::InitializeDXGIDevice() {
			// ファクトリの生成
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
				if (SUCCEEDED(D3D12CreateDevice(selectedAdapter, level, IID_PPV_ARGS(&m_device)))) {
					featureLevel = level;
					result = S_OK;
					break;
				}
			}
			return result;
		}


		//@―---------------------------------------------------------------------------
		//! @brief  コマンドの初期化
		//@―---------------------------------------------------------------------------
		HRESULT D3D12Device::InitializeCommand() {

			// コマンドアロケータを生成
			auto result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				return result;
			}


			// コマンドキューの生成
			D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
			cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる
			cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウトなし
			cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// コマンドキューの優先度
			cmdQueueDesc.NodeMask = 0;										// GPUが1つの時は0、複数の時は識別用のbitを指定

			result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()));

		}

    }// namespace graphic
}// namespace ob
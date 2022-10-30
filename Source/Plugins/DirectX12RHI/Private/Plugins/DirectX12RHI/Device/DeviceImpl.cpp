//***********************************************************
//! @file
//! @brief		デバイス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DeviceImpl.h"
#include<Plugins/DirectX12RHI/Utility/Utility.h>
#include<Plugins/DirectX12RHI/SwapChain/SwapChainImpl.h>
#include<Plugins/DirectX12RHI/Command/CommandListImpl.h>
#include<Plugins/DirectX12RHI/Command/CommandQueue.h>
#include<Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>
#include<Plugins/DirectX12RHI/PipelineState/PipelineStateImpl.h>
#include<Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include<Plugins/DirectX12RHI/Texture/RenderTargetImpl.h>
#include<Plugins/DirectX12RHI/Shader/ShaderImpl.h>
#include<Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include<Plugins/DirectX12RHI/Descriptor/DescriptorTableImpl.h>
#include<Plugins/DirectX12RHI/Buffer/BufferImpl.h>


#ifdef OB_DEBUG
#include <Plugins/DirectX12RHI/Utility/PIXModule.h>
#endif

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
		:m_featureLevel(featureLevel) {
		initialize();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::~DeviceImpl() {
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::isValid()const {
		return m_device && m_commandQueue;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドをシステムキューに追加
	//@―---------------------------------------------------------------------------
	void DeviceImpl::entryCommandList(const CommandList& commandList) {
		m_commandQueue->entryCommandList(commandList);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void DeviceImpl::update() {
		m_commandQueue->execute();
		m_commandQueue->wait();

		// 

		// コマンドをGPUに送信

		//m_commandQueue->ExecuteCommandLists((UINT)cmdlists.size(), cmdlists.data());
		//m_commandQueue->Signal(m_fence.Get(), ++m_fenceVal);
		//if (m_fence->GetCompletedValue() < m_fenceVal)
		//{
		//	auto event = CreateEvent(nullptr, false, false, nullptr);
		//	m_fence->SetEventOnCompletion(m_fenceVal, event);
		//	WaitForSingleObject(event, INFINITE);
		//	CloseHandle(event);
		//}
		//
		//
		//m_cmdAllocator->Reset();
		//m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
		//
		//// 描画結果を画面に反映
		//m_swapchain->Present(1, 0);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  スワップ・チェーンを生成
	//@―---------------------------------------------------------------------------
	ISwapChain* DeviceImpl::createSwapChain(const SwapchainDesc& desc) {
		return new SwapChainImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドリストを生成
	//@―---------------------------------------------------------------------------
	ICommandList* DeviceImpl::createCommandList(const CommandListDesc& desc) {
		return new CommandListImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ルートシグネチャを生成
	//@―---------------------------------------------------------------------------
	IRootSignature* DeviceImpl::createRootSignature(const RootSignatureDesc& desc) {
		return new RootSignatureImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステートを生成
	//@―---------------------------------------------------------------------------
	IPipelineState* DeviceImpl::createPipelineState(const PipelineStateDesc& desc) {
		return new PipelineStateImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::ITexture* DeviceImpl::createTexture(const TextureDesc& desc) {
		return new TextureImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::ITexture* DeviceImpl::createTexture(BlobView blob) {
		return new TextureImpl(*this, blob);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーテクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IRenderTarget* DeviceImpl::createRenderTarget(const RenderTargetDesc& desc) {
		return new RenderTargetImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バッファーを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IBuffer* DeviceImpl::createBuffer(const BufferDesc& desc) {
		return new BufferImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IShader* DeviceImpl::createShader(const String& code, ShaderStage stage) {
		return new ShaderImpl(code, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IShader* DeviceImpl::createShader(const Blob& binary, ShaderStage stage) {
		return new ShaderImpl(binary, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブルを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IDescriptorTable* DeviceImpl::createDescriptorTable(DescriptorHeapType type, s32 elementNum) {
		auto index = enum_cast(type);
		if (!is_in_range(index, m_descriptorHeaps))return nullptr;
		return new DescriptorTableImpl(*m_descriptorHeaps[index], type, elementNum);
	}



	//@―---------------------------------------------------------------------------
	//! @brief  システム・コマンド・キューを取得
	//@―---------------------------------------------------------------------------
	ComPtr<ID3D12CommandQueue>& DeviceImpl::getCommandQueue() {
		return m_commandQueue->getNative();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  システム・コマンド・リストを取得
	//@―---------------------------------------------------------------------------
	//ComPtr<ID3D12GraphicsCommandList>& DeviceImpl::getSystemCommandList() {
	//	return m_systemCmdList;
	//}


	//@―---------------------------------------------------------------------------
	//! @brief          ハンドルをアロケート
	//! 
	//! @param type     ヒープタイプ
	//! @param handle   アロケート先ハンドル
	//! @param size     割り当て個数
	//@―---------------------------------------------------------------------------
	void DeviceImpl::allocateHandle(DescriptorHeapType type, DescriptorHandle& handle, s32 size) {
		auto index = enum_cast(type);
		if (!is_in_range(index, m_descriptorHeaps)) {
			OB_ASSERT("不正なDescriptorHeapType");
		}
		OB_CHECK_ASSERT(m_descriptorHeaps[index], "DescriptorHeapが生成されていません。");
		m_descriptorHeaps[index]->allocateHandle(handle, size);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  SetDescriptorHeaps コマンドを積む
	//@―---------------------------------------------------------------------------
	void DeviceImpl::setDescriptorHeaps(CommandListImpl& cmdList) {
		ID3D12DescriptorHeap* pHeaps[] = {
			m_descriptorHeaps[enum_cast(DescriptorHeapType::CBV_SRV_UAV)]->getNative().Get(),
			m_descriptorHeaps[enum_cast(DescriptorHeapType::Sampler)]->getNative().Get(),
		};
		cmdList.getNative()->SetDescriptorHeaps((UINT)std::size(pHeaps), pHeaps);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initialize() {

		if (!initializeDXGIDevice())return false;

		m_commandQueue = std::make_unique<CommandQueue>(*this);
		OB_DEBUG_CONTEXT(m_commandQueue->setName(TC("SystemCommandQueue")));

		//if (!initializeCommand())return false;
		//if (!initializeFence())return false;
		if (!initializeDescriptorHeaps())return false;
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  DXGIDeviceの初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeDXGIDevice() {
		HRESULT result;
		UINT flagsDXGI = 0;
#if OB_DEBUG
		// DirectX12のデバッグレイヤーを有効にする
		{
			ComPtr<ID3D12Debug>	debugController;
			result = ::D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			if (FAILED(result)) {
				Utility::outputFatalLog(result, TC("D3D12GetDebugInterface()"));
				return false;
			}
			debugController->EnableDebugLayer();
		}
#endif

		// ファクトリの生成
		result = ::CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("CreateDXGIFactory2()"));
			return false;
		}

		// アダプターの列挙し、メモリ量が最大のグラフィックボードを選択
		Array<IDXGIAdapter*> adapters;

		ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
		ComPtr<IDXGIAdapter> selectedAdapter = nullptr;
		SIZE_T maxVideoCardMemory = 0;
		for (int i = 0; m_dxgiFactory->EnumAdapters(i, tmpAdapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++) {
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
			if (SUCCEEDED(::D3D12CreateDevice(selectedAdapter.Get(), level, IID_PPV_ARGS(&m_device)))) {
				featureLevel = level;
				result = S_OK;
				break;
			}
		}
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("D3D12CreateDevice()"));
			return false;
		}
		OB_DEBUG_CONTEXT(m_device->SetName(L"System Device"));




		ComPtr<ID3D12InfoQueue> infoQueue;
		if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(infoQueue.ReleaseAndGetAddressOf())))) {
			// 不必要な警告をフィルター
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
			};
			D3D12_MESSAGE_SEVERITY severities[] = {
			  D3D12_MESSAGE_SEVERITY_INFO,
			};
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = (UINT)std::size(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = (UINT)std::size(severities);
			filter.DenyList.pSeverityList = severities;

			infoQueue->PushStorageFilter(&filter);


			// D3D12 エラー発生時にブレーク
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		}

		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドの初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeCommand() {

		// コマンドアロケータを生成
		//auto result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf()));
		//if (FAILED(result)) {
		//	Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommandAllocator()"));
		//
		//	// DeviceRemoved
		//	if (result == 0x887a0005) {
		//		result = m_device->GetDeviceRemovedReason();
		//		Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommandAllocator()"));
		//	}
		//}
		//
		//// システム・コマンドリストを生成
		//result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(m_systemCmdList.ReleaseAndGetAddressOf()));
		//if (FAILED(result)) {
		//	Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommandList()"));
		//	return false;
		//}
		//
		//
		//// コマンドキューの生成
		//D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
		//cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる
		//cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウトなし
		//cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// コマンドキューの優先度
		//cmdQueueDesc.NodeMask = 0;										// GPUが1つの時は0、複数の時は識別用のbitを指定
		//
		//result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()));
		//if (FAILED(result)) {
		//	Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommandQueue()"));
		//	return false;
		//}
		//
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  フェンスを初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeFence() {
		//m_fenceVal = 0;
		//auto result = m_device->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		//if (FAILED(result)) {
		//	Utility::outputFatalLog(result, TC("ID3D12Device::CreateFence()"));
		//	return false;
		//}
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ビデオカード情報を初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeVideoCardInfo() {
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタヒープを初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeDescriptorHeaps() {
		m_descriptorHeaps.resize((size_t)enum_cast(DescriptorHeapType::DSV) + 1u);

		m_descriptorHeaps[enum_cast(DescriptorHeapType::CBV_SRV_UAV)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, 10'000);
		m_descriptorHeaps[enum_cast(DescriptorHeapType::Sampler)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::Sampler, 100);
		m_descriptorHeaps[enum_cast(DescriptorHeapType::RTV)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::RTV, 100);
		m_descriptorHeaps[enum_cast(DescriptorHeapType::DSV)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::DSV, 100);

		OB_DEBUG_CONTEXT(
			m_descriptorHeaps[enum_cast(DescriptorHeapType::CBV_SRV_UAV)]->setName(TC("SystemCBV_SRV_UAVHeap"));
		m_descriptorHeaps[enum_cast(DescriptorHeapType::Sampler)]->setName(TC("SystemSamplerHeap"));
		m_descriptorHeaps[enum_cast(DescriptorHeapType::RTV)]->setName(TC("SystemRTVHeap"));
		m_descriptorHeaps[enum_cast(DescriptorHeapType::DSV)]->setName(TC("SystemDSVHeap"));
		);
		return true;
	}

}// namespace ob::rhi::dx12
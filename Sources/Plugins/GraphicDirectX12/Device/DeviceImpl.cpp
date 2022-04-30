//***********************************************************
//! @file
//! @brief		デバイス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DeviceImpl.h"
#include<Plugins/GraphicDirectX12/Utility/Utility.h>
#include<Plugins/GraphicDirectX12/SwapChain/SwapChainImpl.h>
#include<Plugins/GraphicDirectX12/RenderPass/RenderPassImpl.h>
#include<Plugins/GraphicDirectX12/FrameBuffer/FrameBufferImpl.h>
#include<Plugins/GraphicDirectX12/RootSignature/RootSignatureImpl.h>
#include<Plugins/GraphicDirectX12/PipelineState/PipelineStateImpl.h>
#include<Plugins/GraphicDirectX12/Texture/TextureImpl.h>
#include<Plugins/GraphicDirectX12/Texture/RenderTextureImpl.h>
#include<Plugins/GraphicDirectX12/Shader/ShaderImpl.h>
#include<Plugins/GraphicDirectX12/Descriptor/DescriptorHeap.h>
#include<Plugins/GraphicDirectX12/Descriptor/DescriptorTableImpl.h>


namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
		:m_featureLevel(featureLevel) {
		initialize();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::isValid()const{
		return m_device && m_commandQueue;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void DeviceImpl::update() {
		// 

		// コマンドをGPUに送信

		// 描画結果を画面に反映
	}


	//@―---------------------------------------------------------------------------
	//! @brief  スワップ・チェーンを生成
	//@―---------------------------------------------------------------------------
	ISwapChain* DeviceImpl::createSwapChain(const SwapchainDesc& desc) {
		return new SwapChainImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーパスを生成
	//@―---------------------------------------------------------------------------
	IRenderPass* DeviceImpl::createRenderPass(const RenderPassDesc& desc) {
		return new RenderPassImpl(desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  フレームバッファを生成
	//@―---------------------------------------------------------------------------
	IFrameBuffer* DeviceImpl::createFrameBuffer(const FrameBufferDesc& desc) {
		return new FrameBufferImpl(desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ルートシグネチャを生成
	//@―---------------------------------------------------------------------------
	IRootSignature* DeviceImpl::createRootSignature(const RootSignatureDesc& desc) {
		return new RootSignatureImpl(*this,desc);
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
	ob::graphic::ITexture* DeviceImpl::createTexture(const TextureDesc& desc) {
		return new TextureImpl(*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーテクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IRenderTexture* DeviceImpl::createRenderTexture(const gsl::span<TextureDesc> targets, const TextureDesc& depth) {
		return new RenderTextureImpl(*this, targets, depth);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  バッファーを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IBuffer* DeviceImpl::createBuffer(const BufferDesc& desc) {
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IShader* DeviceImpl::createShader(const String& code, ShaderStage stage) {
		return new ShaderImpl(code, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IShader* DeviceImpl::createShader(const Blob& binary, ShaderStage stage) {
		return new ShaderImpl(binary, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブルを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IDescriptorTable* DeviceImpl::createDescriptorTable(DescriptorHeapType type, s32 elementNum) {
		auto index = enum_cast(type);
		if (!is_in_range(index, m_descriptorHeaps))return nullptr;
		return new DescriptorTableImpl(*m_descriptorHeaps[index],type,elementNum);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initialize() {
		if (!initializeDXGIDevice())return false;
		if (!initializeCommand())return false;
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
				Utility::outputErrorLog(result, TC("D3D12GetDebugInterface()"));
				return false;
			}
			debugController->EnableDebugLayer();
		}
#endif

		// ファクトリの生成
		result = ::CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("CreateDXGIFactory2()"));
			return false;
		}

		// アダプターの列挙し、メモリ量が最大のグラフィックボードを選択
		std::vector<IDXGIAdapter*> adapters;

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
			Utility::outputErrorLog(result, TC("D3D12CreateDevice()"));
			return false;
		}

		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドの初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeCommand() {

		// コマンドアロケータを生成
		auto result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("ID3D12Device::CreateCommandAllocator()"));

			// DeviceRemoved
			if (result == 0x887a0005) {
				result = m_device->GetDeviceRemovedReason();
				Utility::outputErrorLog(result, TC("ID3D12Device::CreateCommandAllocator()"));
			}
		}

		// システム・コマンドリストを生成
		result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(m_systemCmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("ID3D12Device::CreateCommandList()"));
			return false;
		}


		// コマンドキューの生成
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウトなし
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// コマンドキューの優先度
		cmdQueueDesc.NodeMask = 0;										// GPUが1つの時は0、複数の時は識別用のbitを指定

		result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("ID3D12Device::CreateCommandQueue()"));
			return false;
		}

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
		m_descriptorHeaps.resize(enum_cast(DescriptorHeapType::Max));
		m_descriptorHeaps[enum_cast(DescriptorHeapType::CBV_SRV_UAV)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, 10'000);
		m_descriptorHeaps[enum_cast(DescriptorHeapType::Sampler)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::Sampler, 100);
		m_descriptorHeaps[enum_cast(DescriptorHeapType::RTV)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::RTV, 100);
		m_descriptorHeaps[enum_cast(DescriptorHeapType::DSV)] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::DSV, 100);
		return true;
	}

}// namespace ob::graphic::dx12
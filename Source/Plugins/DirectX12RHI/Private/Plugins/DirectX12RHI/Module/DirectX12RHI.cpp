//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Module/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Display/DisplayImpl.h>
#include <Plugins/DirectX12RHI/Command/CommandListImpl.h>
#include <Plugins/DirectX12RHI/Command/CommandQueue.h>
#include <Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/DirectX12RHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Plugins/DirectX12RHI/RenderPass/RenderPassImpl.h>
#include <Plugins/DirectX12RHI/FrameBuffer/FrameBufferImpl.h>
#include <Plugins/DirectX12RHI/Shader/ShaderImpl.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorTableImpl.h>
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>

#ifdef OB_DEBUG
#include <Plugins/DirectX12RHI/Utility/PIXModule.h>
#endif

#define SAFE_CREATE(type,...)			\
	auto p = new type(__VA_ARGS__);		\
	if(p->isValid() == false) {			\
		delete p;						\
		return {};						\
	}									\
	return p;							

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DirectX12RHI::DirectX12RHI(ob::rhi::Config* config, platform::WindowManager&)
		: m_config(config ? *config : ob::rhi::Config{})
		, m_objectManager(m_config.frameBufferCount)
	{
		OB_DEBUG_CONTEXT(
			if (m_config.enablePIX) {
				m_pixModule = std::make_unique<PIXModule>();
			}
		);
		initialize();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DirectX12RHI::~DirectX12RHI() {
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool DirectX12RHI::isValid()const {
		return m_device && m_commandQueue;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドをシステムキューに追加
	//@―---------------------------------------------------------------------------
	void DirectX12RHI::entryCommandList(const CommandList& commandList) {
		m_commandQueue->entryCommandList(commandList);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void DirectX12RHI::update() {
		m_commandQueue->execute();
		m_commandQueue->wait();

		m_objectManager->update();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーパスを生成を生成
	//@―---------------------------------------------------------------------------
	Ref<RenderPass> DirectX12RHI::createRenderPass(const RenderPassDesc& desc) {
		SAFE_CREATE(RenderPassImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  フレームバッファを生成
	//@―---------------------------------------------------------------------------
	Ref<FrameBuffer> DirectX12RHI::createFrameBuffer(const FrameBufferDesc& desc) {
		SAFE_CREATE(FrameBufferImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  スワップ・チェーンを生成
	//@―---------------------------------------------------------------------------
	Ref<Display> DirectX12RHI::createDisplay(const DisplayDesc& desc) {
		SAFE_CREATE(DisplayImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドリストを生成
	//@―---------------------------------------------------------------------------
	Ref<CommandList> DirectX12RHI::createCommandList(const CommandListDesc& desc) {
		SAFE_CREATE(CommandListImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ルートシグネチャを生成
	//@―---------------------------------------------------------------------------
	Ref<RootSignature> DirectX12RHI::createRootSignature(const RootSignatureDesc& desc) {
		SAFE_CREATE(RootSignatureImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステートを生成
	//@―---------------------------------------------------------------------------
	Ref<PipelineState> DirectX12RHI::createPipelineState(const PipelineStateDesc& desc) {
		SAFE_CREATE(PipelineStateImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<Texture> DirectX12RHI::createTexture(const TextureDesc& desc) {
		SAFE_CREATE(TextureImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<Texture> DirectX12RHI::createTexture(Size size, Span<IntColor> colors) {
		SAFE_CREATE(TextureImpl, *this, size, colors);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<Texture> DirectX12RHI::createTexture(BlobView blob, StringView name) {
		SAFE_CREATE(TextureImpl, *this, blob, name);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーテクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<RenderTexture> DirectX12RHI::createRenderTexture(const RenderTextureDesc& desc) {
		SAFE_CREATE(TextureImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バッファーを生成
	//@―---------------------------------------------------------------------------
	Ref<Buffer> DirectX12RHI::createBuffer(const BufferDesc& desc) {
		SAFE_CREATE(BufferImpl, *this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	Ref<Shader> DirectX12RHI::createShader(const String& code, ShaderStage stage) {
		SAFE_CREATE(ShaderImpl, code, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	Ref<Shader> DirectX12RHI::createShader(BlobView binary, ShaderStage stage) {
		SAFE_CREATE(ShaderImpl, binary, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブルを生成
	//@―---------------------------------------------------------------------------
	Ref<DescriptorTable> DirectX12RHI::createDescriptorTable(DescriptorHeapType type, s32 elementNum) {
		if (m_descriptorHeaps.find(type) == m_descriptorHeaps.end())return nullptr;
		SAFE_CREATE(DescriptorTableImpl, *m_descriptorHeaps[type], type, elementNum);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  システム・コマンド・キューを取得
	//@―---------------------------------------------------------------------------
	ComPtr<ID3D12CommandQueue>& DirectX12RHI::getCommandQueue() {
		return m_commandQueue->getNative();
	}


	//@―---------------------------------------------------------------------------
	//! @brief          ハンドルをアロケート
	//! 
	//! @param type     ヒープタイプ
	//! @param handle   アロケート先ハンドル
	//! @param size     割り当て個数
	//@―---------------------------------------------------------------------------
	void DirectX12RHI::allocateHandle(DescriptorHeapType type, DescriptorHandle& handle, s32 size) {
		if (m_descriptorHeaps.find(type) == m_descriptorHeaps.end()) {
			OB_ABORT("不正なDescriptorHeapType");
		}
		m_descriptorHeaps[type]->allocateHandle(handle, size);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  SetDescriptorHeaps コマンドを積む
	//@―---------------------------------------------------------------------------
	void DirectX12RHI::setDescriptorHeaps(CommandListImpl& cmdList) {
		ID3D12DescriptorHeap* pHeaps[] = {
			m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->getNative().Get(),
			m_descriptorHeaps[DescriptorHeapType::Sampler]->getNative().Get(),
		};
		cmdList.getNative()->SetDescriptorHeaps((UINT)std::size(pHeaps), pHeaps);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  初期化
	//@―---------------------------------------------------------------------------
	bool DirectX12RHI::initialize() {

		if (!initializeDXGIDevice())return false;

		m_commandQueue = std::make_unique<CommandQueue>(*this);
		OB_DEBUG_CONTEXT(m_commandQueue->setName(TC("SystemCommandQueue")));

		if (!initializeDescriptorHeaps())return false;

		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  DXGIDeviceの初期化
	//@―---------------------------------------------------------------------------
	bool DirectX12RHI::initializeDXGIDevice() {
		HRESULT result;
		UINT flagsDXGI = 0;
#if OB_DEBUG
		// DirectX12のデバッグレイヤーを有効にする
		{
			ComPtr<ID3D12Debug>	debugController;
			result = ::D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			if (FAILED(result)) {
				Utility::OutputFatalLog(result, TC("D3D12GetDebugInterface()"));
				return false;
			}
			debugController->EnableDebugLayer();
		}
#endif

		// ファクトリの生成
		result = ::CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("CreateDXGIFactory2()"));
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
			Utility::OutputFatalLog(result, TC("D3D12CreateDevice()"));
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
	//! @brief  ビデオカード情報を初期化
	//@―---------------------------------------------------------------------------
	bool DirectX12RHI::initializeVideoCardInfo() {
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタヒープを初期化
	//@―---------------------------------------------------------------------------
	bool DirectX12RHI::initializeDescriptorHeaps() {

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, 10'000);
		m_descriptorHeaps[DescriptorHeapType::Sampler] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::Sampler, 256);
		m_descriptorHeaps[DescriptorHeapType::RTV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::RTV, 256);
		m_descriptorHeaps[DescriptorHeapType::DSV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::DSV, 256);

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->setName(TC("SystemCBV_SRV_UAVHeap"));
		m_descriptorHeaps[DescriptorHeapType::Sampler]->setName(TC("SystemSamplerHeap"));
		m_descriptorHeaps[DescriptorHeapType::RTV]->setName(TC("SystemRTVHeap"));
		m_descriptorHeaps[DescriptorHeapType::DSV]->setName(TC("SystemDSVHeap"));

		return true;
	}

}// namespace ob::rhi::dx12
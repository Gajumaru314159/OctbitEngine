//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/SwapChain/DirectX12SwapChain.h>
#include <Plugins/DirectX12RHI/Command/DirectX12CommandList.h>
#include <Plugins/DirectX12RHI/Command/CommandQueue.h>
#include <Plugins/DirectX12RHI/RootSignature/DirectX12RootSignature.h>
#include <Plugins/DirectX12RHI/PipelineState/DirectX12PipelineState.h>
#include <Plugins/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Plugins/DirectX12RHI/Shader/DirectX12Shader.h>
#include <Plugins/DirectX12RHI/Sampler/DirectX12Sampler.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorLayout.h>
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorTable.h>
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/GraphicFile/DirectX12GraphicFile.h>
#include <Framework/Platform/System.h>

#ifdef OB_DEBUG
#include <pix3.h>
#include <Plugins/DirectX12RHI/Utility/PIXModule.h>
#endif

#define SAFE_CREATE(type,type_impl,...)			\
	Ref<type> p = new type_impl(__VA_ARGS__);	\
	if(p.cast<type_impl>()->isValid() == false) p = {};			\
	return p;		

namespace ob::rhi {

	//! @brief  DirectX12RHIの起動に必要なサービスを登録
	void DirectX12RHI::Inject(ServiceInjector& injector) {
		injector.bind<DirectX12RHI>().as<RHI>();
		platform::RegisterPlatformService(injector);
	}

	//! @brief  コンストラクタ
	DirectX12RHI::DirectX12RHI(platform::WindowManager&, GraphicObjectManager& objectManager, RHIConfig* config,DirectX12RHIConfig* dx12config)
		: RHI(objectManager,config)
		, m_config(config ? *config : RHIConfig{})
		, m_dx12config(dx12config ? *dx12config : DirectX12RHIConfig{})
	{
		OB_DEBUG_CONTEXT(
			if (m_dx12config.enablePIX) {
				m_pixModule = std::make_unique<PIXModule>();
			}
		);
		initialize();
	}


	//! @brief  デストラクタ
	DirectX12RHI::~DirectX12RHI() {
		clearCommands();

		// GraphicsObjectを持っているものは先に破棄
		m_textureUploader.destruct();
		m_bufferUploader.destruct();
		m_copyCommandList = {};

		finalize();
	}


	//! @brief  妥当な状態か
	bool DirectX12RHI::isValid()const {
		return m_device && m_commandQueue;
	}


	//! @brief  コマンドをシステムキューに追加
	void DirectX12RHI::entryCommandList(const Ref<CommandList>& commandList) {
		if (commandList) {
			m_commandQueue->entryCommandList(*commandList);
		}
	}


	//! @brief  更新
	void DirectX12RHI::update() {

		{
			m_copyCommandList->begin();
			m_bufferUploader->update(*const_cast<DirectX12CommandList*>(m_copyCommandList.cast<DirectX12CommandList>())->getNative());
			m_textureUploader->update(*const_cast<DirectX12CommandList*>(m_copyCommandList.cast<DirectX12CommandList>())->getNative());
			m_copyCommandList->end();

			m_commandQueue->entryCommandListTop(*m_copyCommandList);
			// m_copyCommandList->wait();
		}

		m_commandQueue->execute();
		m_commandQueue->wait();

		RHI::update();
	}


	//! @brief  コマンドリストを生成
	Ref<SwapChain> DirectX12RHI::createSwapChain(const SwapChainDesc& desc) {
		SAFE_CREATE(SwapChain, DirectX12SwapChain, *this, desc);
	}


	//! @brief  コマンドリストを生成
	Ref<CommandList> DirectX12RHI::createCommandList(const CommandListDesc& desc) {
		SAFE_CREATE(CommandList, DirectX12CommandList, *this, desc);
	}


	//! @brief  ルートシグネチャを生成
	Ref<RootSignature> DirectX12RHI::createRootSignature(const RootSignatureDesc& desc) {
		SAFE_CREATE(RootSignature, DirectX12RootSignature, *this, desc);
	}


	//! @brief  パイプラインステートを生成
	Ref<PipelineState> DirectX12RHI::createPipelineState(const PipelineStateDesc& desc) {
		SAFE_CREATE(PipelineState, DirectX12PipelineState, *this, desc);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12RHI::createTexture(const TextureDesc& desc) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, desc);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12RHI::createTexture(StringView name, TextureType type, Size size, Span<const IntColor> colors) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, name, type, size, colors);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12RHI::createTexture(StringView name, BlobView blob) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, name, blob);
	}


	//! @brief  レンダーテクスチャを生成
	Ref<RenderTexture> DirectX12RHI::createRenderTexture(const RenderTextureDesc& desc) {
		SAFE_CREATE(RenderTexture, DirectX12Texture, *this, desc);
	}


	//! @brief  サンプラーを生成
	Ref<Sampler> DirectX12RHI::createSampler(const SamplerDesc& desc) {
		SAFE_CREATE(Sampler, DirectX12Sampler, *this, desc);
	}


	//! @brief  バッファーを生成
	Ref<Buffer> DirectX12RHI::createBuffer(const BufferDesc& desc) {
		SAFE_CREATE(Buffer, DirectX12Buffer, *this, desc);
	}


	//! @brief  シェーダをコンパイル
	Ref<Shader> DirectX12RHI::compileShader(const ShaderCompileDesc& desc) {
		SAFE_CREATE(Shader, DirectX12Shader, *this, desc);
	}

	
	//! @brief  シェーダをロード
	Ref<Shader> DirectX12RHI::loadShader(BlobView binary, ShaderStage stage) {
		SAFE_CREATE(Shader, DirectX12Shader, binary, stage);
	}


	//! @brief  デスクリプタ・レイアウトを生成 
	Ref<DescriptorLayout> DirectX12RHI::createDescriptorLayout(const DescriptorLayoutDesc& desc) {
		SAFE_CREATE(DescriptorLayout, DirectX12DescriptorLayout, desc);
	}

	//! @brief  デスクリプタ・テーブルを生成
	Ref<DescriptorTable> DirectX12RHI::createDescriptorTable(const DescriptorTableDesc& desc) {
		SAFE_CREATE(DescriptorTable, DirectX12DescriptorTable, *this, desc, *m_descriptorHeaps.at(DescriptorHeapType::Sampler),*m_descriptorHeaps.at(DescriptorHeapType::CBV_SRV_UAV));
	}


	//! @brief  GraphicFileHandleを生成
	Ref<GraphicFileHandle> DirectX12RHI::createGraphicFileHandle(StringView path) {
		if (g_dsfactory == nullptr) return nullptr;
		auto p = new DirectX12GraphicFileHandle(*g_dsfactory.Get(), path);
		if (p->isValid() == false) return nullptr;
		return p;
	}

	//! @brief  GraphicFileEventを生成
	Ref<GraphicFileEvent> DirectX12RHI::createGraphicFileEvent() {
		if (g_dsfactory == nullptr) return nullptr;
		auto p = new DirectX12GraphicFileEvent();
		if (p->isValid() == false) return nullptr;
		return p;
	}

	//! @brief  GraphicFileQueueを生成
	Ref<GraphicFileQueue> DirectX12RHI::createGraphicFileQueue(const GraphicFileQueueDesc& desc) {
		if (g_dsfactory == nullptr) return nullptr;
		auto p = new DirectX12GraphicFileQueue(*m_device.Get(), *g_dsfactory.Get(), desc);
		if (p->isValid() == false) return nullptr;
		return p;
	}

	//! @brief  GraphicFile用のファイルを生成する
	bool DirectX12RHI::generateGraphicFile(StringView input, StringView output, s32 compressionLevel) {
		return DirectX12GraphicFile::Generate(*m_device.Get(), input, output,compressionLevel);
	}

	//! @brief  プラットフォームごとのGraphicFileから事前情報を取得
	Vector<GraphicFileMipInfo> DirectX12RHI::prepareGraphicFile(StringView path) {
		return DirectX12GraphicFile::Prepare(path);
	}

	//! @brief サポートしているテクスチャフォーマットか 
	bool DirectX12RHI::supports(TextureFormat format, TextureType type)const {

		if (format == TextureFormat::Unknown)return false;

		D3D12_FEATURE_DATA_FORMAT_SUPPORT result;
		result.Format = TypeConverter::Convert(format);
		result.Support1 = D3D12_FORMAT_SUPPORT1_NONE;
		if (type == TextureType::Texture1D) result.Support1 = D3D12_FORMAT_SUPPORT1_TEXTURE1D;
		if (type == TextureType::Texture2D) result.Support1 = D3D12_FORMAT_SUPPORT1_TEXTURE2D;
		if (type == TextureType::Texture3D) result.Support1 = D3D12_FORMAT_SUPPORT1_TEXTURE3D;
		if (type == TextureType::Cube) result.Support1 = D3D12_FORMAT_SUPPORT1_TEXTURE3D;
		result.Support2 = D3D12_FORMAT_SUPPORT2_NONE;

		return SUCCEEDED(m_device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &result, sizeof(result)));
	}

	//! @brief サポートしているテクスチャフォーマットか 
	bool DirectX12RHI::supportsForRenderTexture(TextureFormat format)const {
		
		if (format == TextureFormat::Unknown)return false;

		D3D12_FEATURE_DATA_FORMAT_SUPPORT result;
		result.Format = TypeConverter::Convert(format);
		result.Support1 = D3D12_FORMAT_SUPPORT1_RENDER_TARGET;
		result.Support2 = D3D12_FORMAT_SUPPORT2_NONE;

		return SUCCEEDED(m_device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &result, sizeof(result)));
	}

	//! @brief サポートしているシェーダーステージか 
	bool DirectX12RHI::supports(ShaderStage stage)const {
		// TODO サポートするシェーダーステージのチェック
		return true;
	}

	//! @brief  システム・コマンド・キューを取得
	ComPtr<ID3D12CommandQueue>& DirectX12RHI::getCommandQueue() {
		return m_commandQueue->getNative();
	}


	//! @brief          ハンドルをアロケート
	//! 
	//! @param type     ヒープタイプ
	//! @param handle   アロケート先ハンドル
	//! @param size     割り当て個数
	void DirectX12RHI::allocateHandle(DescriptorHeapType type, DescriptorHandle& handle, s32 size) {
		if (m_descriptorHeaps.find(type) == m_descriptorHeaps.end()) {
			OB_ABORT("不正なDescriptorHeapType");
		}
		m_descriptorHeaps[type]->allocateHandle(handle, size);
	}


	//! @brief  SetDescriptorHeaps コマンドを積む
	void DirectX12RHI::setDescriptorHeaps(DirectX12CommandList& cmdList) {
		ID3D12DescriptorHeap* pHeaps[] = {
			m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->getNative().Get(),
			m_descriptorHeaps[DescriptorHeapType::Sampler]->getNative().Get(),
		};
		cmdList.getNative()->SetDescriptorHeaps((UINT)std::size(pHeaps), pHeaps);
	}


	//! @brief  IDStorageFactoryを取得
	ComPtr<IDStorageFactory>& DirectX12RHI::getDirectStorageFactory() {
		return g_dsfactory;
	}


	//! @brief  初期化
	bool DirectX12RHI::initialize() {

		if (!initializeDXGIDevice())return false;

		m_commandQueue = std::make_unique<CommandQueue>(*this);
		OB_DEBUG_CONTEXT(m_commandQueue->setName("SystemCommandQueue"));

		if (!initializeDescriptorHeaps())return false;

		if (!initializeShaderCompiler())return false;

		if (!initializeUploaders())return false;

		if (!initializeDirectStorage())return false;

		return true;
	}


	//! @brief  DXGIDeviceの初期化
	bool DirectX12RHI::initializeDXGIDevice() {
		HRESULT result;
		UINT flagsDXGI = 0;
#if OB_DEBUG
		// DirectX12のデバッグレイヤーを有効にする
		if (m_dx12config.enableDebugLayer) {
			ComPtr<ID3D12Debug>	debugController;
			result = ::D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			if (FAILED(result)) {
				Utility::OutputFatalLog(result, "D3D12GetDebugInterface()");
				return false;
			}
			debugController->EnableDebugLayer();
		}
#endif

		// ファクトリの生成
		result = ::CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "CreateDXGIFactory2()");
			return false;
		}

		// アダプターの列挙し、メモリ量が最大のグラフィックボードを選択
		Vector<IDXGIAdapter*> adapters;

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
			Utility::OutputFatalLog(result, "D3D12CreateDevice()");
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
			if (m_dx12config.breakWithWarning)infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		}

		return true;
	}


	//! @brief  ビデオカード情報を初期化
	bool DirectX12RHI::initializeVideoCardInfo() {
		return true;
	}


	//! @brief  デスクリプタヒープを初期化
	bool DirectX12RHI::initializeDescriptorHeaps() {

		bool readable = false;
		bool writable = true;

		// TODO Tierチェック
		// Samplerの割り当て割合調整

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1);
		m_descriptorHeaps[DescriptorHeapType::RTV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::RTV, 256);
		m_descriptorHeaps[DescriptorHeapType::DSV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::DSV, 256);
		m_descriptorHeaps[DescriptorHeapType::Sampler] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::Sampler, D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE / 2 );
		m_descriptorHeaps[DescriptorHeapType::SamplerCopyable] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::SamplerCopyable, D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE / 2);

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->setName("SystemCBV_SRV_UAVHeap");
		m_descriptorHeaps[DescriptorHeapType::RTV]->setName("SystemRTVHeap");
		m_descriptorHeaps[DescriptorHeapType::DSV]->setName("SystemDSVHeap");
		m_descriptorHeaps[DescriptorHeapType::Sampler]->setName("SystemSamplerHeap");
		m_descriptorHeaps[DescriptorHeapType::SamplerCopyable]->setName("SystemSamplerCopyableHeap");

		return true;
	}

	//! @brief  シェーダコンパイラーを初期化
	bool DirectX12RHI::initializeShaderCompiler() {

		HRESULT result;

		result = ::DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_shaderCompiler));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "DxcCreateInstance()");
			return false;
		}

		result = ::DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_shaderUtils));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "DxcCreateInstance()");
			return false;
		}

		// NOTE FileIOをフックする場合は、IDxcIncludeHandlerを継承したカスタムハンドラーを生成する
		result = m_shaderUtils->CreateDefaultIncludeHandler(m_shaderIncludeHandler.GetAddressOf());
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "CreateDefaultIncludeHandler()");
			return false;
		}

		return true;
	}

	//! @brief  アップデータを初期化
	bool DirectX12RHI::initializeUploaders() {

		CommandListDesc desc;
		desc.name = "CopyCommandList";
		desc.type = CommandListType::Graphic;
		m_copyCommandList = createCommandList(desc);
					
		size_t blockSize = 4 * 1024 * 1024;
		m_bufferUploader.construct(*m_device.Get(), blockSize);

		m_textureUploader.construct(*m_device.Get());

		return true;
	}

	//! @brief  DirectStorageを初期化
	bool DirectX12RHI::initializeDirectStorage() {
		if (m_dx12config.enableDirectStorage) {

			DSTORAGE_CONFIGURATION1 config{};
			config.NumSubmitThreads = 8;
			config.NumBuiltInCpuDecompressionThreads = 8;
			::DStorageSetConfiguration1(&config);



			::DStorageGetFactory(IID_PPV_ARGS(g_dsfactory.ReleaseAndGetAddressOf()));
			if (g_dsfactory && m_dx12config.enableDirectStorageDebug) {
				g_dsfactory->SetDebugFlags(
					DSTORAGE_DEBUG_SHOW_ERRORS |
					DSTORAGE_DEBUG_BREAK_ON_ERROR |
					DSTORAGE_DEBUG_RECORD_OBJECT_NAMES
				);
			}

			if (g_dsfactory) {
				g_dsfactory->SetStagingBufferSize(256 * 1024 * 1024);
			}
		}
		// DirectStorageは必須機能ではない
		return true;
	}

	//! @brief  コマンドを実行してクリアする
	void DirectX12RHI::clearCommands() {
		m_commandQueue->execute();
		m_commandQueue->wait();
	}


}
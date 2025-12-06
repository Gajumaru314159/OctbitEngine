//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/DirectX12RHI/DirectX12Device.h>
#include <Framework/DirectX12RHI/Utility/Utility.h>
#include <Framework/DirectX12RHI/Utility/TypeConverter.h>
#include <Framework/DirectX12RHI/SwapChain/DirectX12SwapChain.h>
#include <Framework/DirectX12RHI/Command/DirectX12CommandList.h>
#include <Framework/DirectX12RHI/Command/CommandQueue.h>
#include <Framework/DirectX12RHI/RootSignature/DirectX12RootSignature.h>
#include <Framework/DirectX12RHI/PipelineState/DirectX12PipelineState.h>
#include <Framework/DirectX12RHI/PipelineState/DirectX12ComputePipelineState.h>
#include <Framework/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Framework/DirectX12RHI/Shader/DirectX12Shader.h>
#include <Framework/DirectX12RHI/Sampler/DirectX12Sampler.h>
#include <Framework/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include <Framework/DirectX12RHI/Descriptor/DirectX12DescriptorLayout.h>
#include <Framework/DirectX12RHI/Descriptor/DirectX12DescriptorTable.h>
#include <Framework/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Framework/DirectX12RHI/Buffer/SmallBufferAllocator.h>
#include <Framework/DirectX12RHI/GraphicFile/DirectX12GraphicFile.h>
#include <Framework/Platform/System.h>

#ifdef OB_DEBUG
#include <pix3.h>
#include <Framework/DirectX12RHI/Utility/PIXModule.h>
#endif

#define SAFE_CREATE(type,type_impl,...)			\
	Ref<type> p = new type_impl(__VA_ARGS__);	\
	if(p.cast<type_impl>()->isValid() == false) p = {};			\
	return p;		

namespace ob::rhi {

	//! @brief  DirectX12RHIの起動に必要なサービスを登録
	void DirectX12Device::Inject(ServiceInjector& injector) {
		injector.bind<DirectX12Device>().as<Device>();
		platform::RegisterPlatformService(injector);
	}

	//! @brief  コンストラクタ
	DirectX12Device::DirectX12Device(platform::WindowManager&, GraphicObjectManager& objectManager, const RHIConfig* config, const DirectX12RHIConfig* dx12config)
		: Device(objectManager,config)
		, m_config(config ? *config : RHIConfig{})
		, m_dx12config(dx12config ? *dx12config : DirectX12RHIConfig{})
	{
		OB_DEBUG_CONTEXT(
			if (m_dx12config.enablePIX) {
				m_pixModule = std::make_unique<PIXModule>();
			}
		);

		// WICファイル読込のため、COMを初期化しておく
		auto result = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
		if (FAILED(result)) throw Exception("");

		if (!initializeDXGIDevice()) throw Exception("");

		m_commandQueue = std::make_unique<CommandQueue>(*this);
		OB_DEBUG_CONTEXT(m_commandQueue->setName("SystemCommandQueue"));

		if (!initializeDescriptorHeaps()) throw Exception("");

		if (!initializeShaderCompiler()) throw Exception("");

		if (!initializeUploaders()) throw Exception("");

		if (!initializeDirectStorage()) throw Exception("");

	}


	//! @brief  デストラクタ
	DirectX12Device::~DirectX12Device() {
		clearCommands();

		// GraphicsObjectを持っているものは先に破棄
		m_textureUploader.destruct();
		m_bufferUploader.destruct();
		m_copyCommandList = {};

		OB_DEBUG_CONTEXT(finalizeDebugMessageCallback());

		finalize();

		m_smallBufferAllocators.clear();
	}


	//! @brief  妥当な状態か
	bool DirectX12Device::isValid()const {
		return m_device && m_commandQueue;
	}


	//! @brief  コマンドをシステムキューに追加
	void DirectX12Device::entryCommandList(const Ref<CommandList>& commandList) {
		if (commandList) {
			m_commandQueue->entryCommandList(*commandList);
		}
	}


	//! @brief  更新
	void DirectX12Device::update() {

		// Descriptorコピー
		{
			m_descriptorUploader->update();
			for (auto& [type,heap] : m_descriptorStagingHeaps) {
				heap->reset();
			}
		}

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

		Device::update();
	}


    //! @brief ビデオカード情報を取得  
    Vector<VideoCard> DirectX12Device::getVideoCards() const {  
       Vector<VideoCard> videoCards;  

	   // IDXGIAdapter(VideoCard)を列挙
       ComPtr<IDXGIAdapter1> adapter;  
       for (UINT i = 0; m_dxgiFactory->EnumAdapters1(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i) {  
           DXGI_ADAPTER_DESC1 adapterDesc;  
           if (FAILED(adapter->GetDesc1(&adapterDesc))) {  
               continue;  
           }  

           VideoCard videoCard;  
		   StringEncoder::Encode(adapterDesc.Description, videoCard.name);
           videoCard.memory = static_cast<size_t>(adapterDesc.DedicatedVideoMemory);  
		   videoCard.deviceId = adapterDesc.DeviceId;

		   // IDXGIOutput(VideoOutput)を列挙
           ComPtr<IDXGIOutput> output;  
           for (UINT j = 0; adapter->EnumOutputs(j, output.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++j) {  
               DXGI_OUTPUT_DESC outputDesc;  
               if (FAILED(output->GetDesc(&outputDesc))) {  
                   continue;  
               }  
			   VideoOutput videoOutput;
			   StringEncoder::Encode(outputDesc.DeviceName, videoOutput.name);
			   videoOutput.rect = IntRect(outputDesc.DesktopCoordinates.left, outputDesc.DesktopCoordinates.top, outputDesc.DesktopCoordinates.right, outputDesc.DesktopCoordinates.bottom);
			   videoOutput.isPrimary = (outputDesc.AttachedToDesktop == TRUE);

			   TextureFormat formats[] = { TextureFormat::RGBA8,TextureFormat::RGBA16 };

			   for (auto format : formats) {
				   DXGI_FORMAT dxgiFormat = TypeConverter::Convert(format);
				   Vector<DXGI_MODE_DESC> modes;
				   UINT modeCount = 0;
				   if (SUCCEEDED(output->GetDisplayModeList(dxgiFormat, DXGI_ENUM_MODES_INTERLACED, &modeCount, nullptr))) {
					   modes.resize(modeCount);
					   if (SUCCEEDED(output->GetDisplayModeList(dxgiFormat, DXGI_ENUM_MODES_INTERLACED, &modeCount, modes.data()))) {
						   for (const auto& mode : modes) {
							   VideoOutputMode videoMode;
							   videoMode.width = mode.Width;
							   videoMode.height = mode.Height;
							   videoMode.refreshRate = 1.0 * mode.RefreshRate.Numerator / std::max<UINT>(mode.RefreshRate.Denominator,1);
							   videoMode.format = format;
							   videoOutput.modes.push_back(videoMode);
						   }

						   struct Eq {
							   bool operator()(const VideoOutputMode& lhs, const VideoOutputMode& rhs) const {
								   return lhs.width == rhs.width && lhs.height == rhs.height && lhs.refreshRate == rhs.refreshRate && lhs.format == rhs.format;
							   }
						   };

						   videoOutput.modes.erase(std::unique(videoOutput.modes.begin(), videoOutput.modes.end(), Eq()), videoOutput.modes.end());
					   }
				   }
			   }
			   videoCard.outputs.push_back(videoOutput);
           }  

           videoCards.push_back(videoCard);  
       }  

       return videoCards;  
    }

	//! @brief  コマンドリストを生成
	Ref<SwapChain> DirectX12Device::createSwapChain(const SwapChainDesc& desc) {
		SAFE_CREATE(SwapChain, DirectX12SwapChain, *this, desc);
	}


	//! @brief  コマンドリストを生成
	Ref<CommandList> DirectX12Device::createCommandList(const CommandListDesc& desc) {
		SAFE_CREATE(CommandList, DirectX12CommandList, *this, desc);
	}


	//! @brief  ルートシグネチャを生成
	Ref<RootSignature> DirectX12Device::createRootSignature(const RootSignatureDesc& desc) {
		SAFE_CREATE(RootSignature, DirectX12RootSignature, *this, desc);
	}


	//! @brief  パイプラインステートを生成
	Ref<PipelineState> DirectX12Device::createPipelineState(const PipelineStateDesc& desc) {
		SAFE_CREATE(PipelineState, DirectX12PipelineState, *this, desc);
	}


	//! @brief  コンピュートパイプラインステートを生成
	Ref<ComputePipelineState> DirectX12Device::createComputePipelineState(const ComputePipelineStateDesc& desc) {
		Ref<ComputePipelineState> p = new DirectX12ComputePipelineState(*this, desc);
		if (p.cast<DirectX12ComputePipelineState>()->isValid() == false) p = {}; 
		return p;;
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12Device::createTexture(const TextureDesc& desc) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, desc);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12Device::createTexture(StringView name, TextureType type, Size size, Span<const IntColor> colors) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, name, type, size, colors);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12Device::createTexture(StringView name, BlobView blob) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, name, blob);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> DirectX12Device::createTexture(const TextureViewDesc& desc) {
		SAFE_CREATE(Texture, DirectX12Texture, *this, desc);
	}


	//! @brief  レンダーテクスチャを生成
	Ref<RenderTexture> DirectX12Device::createRenderTexture(const RenderTextureDesc& desc) {
		SAFE_CREATE(RenderTexture, DirectX12Texture, *this, desc);
	}


	//! @brief  サンプラーを生成
	Ref<Sampler> DirectX12Device::createSampler(const SamplerDesc& desc) {
		// TODO descが共通している場合はキャッシュから使いまわす
		SAFE_CREATE(Sampler, DirectX12Sampler, *this, desc);
	}


	//! @brief  SmallBufferAllocatorを取得
	SmallBufferAllocator& DirectX12Device::getSmallBufferAllocator(BufferFlag flag) {
		auto it = m_smallBufferAllocators.find(flag);
		if (it == m_smallBufferAllocators.end()) {
			// BufferFlagに基づいてヒープタイプを決定
			D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT;
			auto allocator = std::make_unique<SmallBufferAllocator>(*m_device.Get(), heapType);
			auto ptr = allocator.get();
			m_smallBufferAllocators[flag] = std::move(allocator);
			return *ptr;
		}
		return *it->second;
	}


	//! @brief  バッファーを生成
	Ref<Buffer> DirectX12Device::createBuffer(const BufferDesc& desc) {
		SAFE_CREATE(Buffer, DirectX12Buffer, *this, desc);
	}


	//! @brief  バッファーを生成
	Ref<Buffer> DirectX12Device::createBuffer(const BufferViewDesc& desc) {
		SAFE_CREATE(Buffer, DirectX12Buffer, *this, desc);
	}


	//! @brief  シェーダをコンパイル
	Ref<Shader> DirectX12Device::compileShader(const ShaderCompileDesc& desc) {
		SAFE_CREATE(Shader, DirectX12Shader, *this, desc);
	}

	
	//! @brief  シェーダをロード
	Ref<Shader> DirectX12Device::loadShader(BlobView binary, ShaderStage stage) {
		SAFE_CREATE(Shader, DirectX12Shader, binary, stage);
	}


	//! @brief  デスクリプタ・レイアウトを生成 
	Ref<DescriptorLayout> DirectX12Device::createDescriptorLayout(const DescriptorLayoutDesc& desc) {
		SAFE_CREATE(DescriptorLayout, DirectX12DescriptorLayout, desc);
	}

	//! @brief  デスクリプタ・テーブルを生成
	Ref<DescriptorTable> DirectX12Device::createDescriptorTable(const DescriptorTableDesc& desc) {
		SAFE_CREATE(DescriptorTable, DirectX12DescriptorTable, *this, desc, *m_descriptorHeaps.at(DescriptorHeapType::Sampler),*m_descriptorHeaps.at(DescriptorHeapType::CBV_SRV_UAV));
	}


	//! @brief  GraphicFileHandleを生成
	Ref<GraphicFileHandle> DirectX12Device::createGraphicFileHandle(StringView path) {
		if (g_dsfactory == nullptr) return nullptr;
		auto p = new DirectX12GraphicFileHandle(*g_dsfactory.Get(), path);
		if (p->isValid() == false) return nullptr;
		return p;
	}

	//! @brief  GraphicFileEventを生成
	Ref<GraphicFileEvent> DirectX12Device::createGraphicFileEvent() {
		if (g_dsfactory == nullptr) return nullptr;
		auto p = new DirectX12GraphicFileEvent();
		if (p->isValid() == false) return nullptr;
		return p;
	}

	//! @brief  GraphicFileQueueを生成
	Ref<GraphicFileQueue> DirectX12Device::createGraphicFileQueue(const GraphicFileQueueDesc& desc) {
		if (g_dsfactory == nullptr) return nullptr;
		auto p = new DirectX12GraphicFileQueue(*m_device.Get(), *g_dsfactory.Get(), desc);
		if (p->isValid() == false) return nullptr;
		return p;
	}

	//! @brief  GraphicFile用のファイルを生成する
	bool DirectX12Device::generateGraphicFile(StringView input, StringView output, s32 compressionLevel) {
		return DirectX12GraphicFile::Generate(*m_device.Get(), input, output,compressionLevel);
	}

	//! @brief  プラットフォームごとのGraphicFileから事前情報を取得
	Vector<GraphicFileMipInfo> DirectX12Device::prepareGraphicFile(StringView path) {
		return DirectX12GraphicFile::Prepare(path);
	}

	//! @brief サポートしているテクスチャフォーマットか 
	bool DirectX12Device::supports(TextureFormat format, TextureType type)const {

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
	bool DirectX12Device::supportsForRenderTexture(TextureFormat format)const {
		
		if (format == TextureFormat::Unknown)return false;

		D3D12_FEATURE_DATA_FORMAT_SUPPORT result;
		result.Format = TypeConverter::Convert(format);
		result.Support1 = D3D12_FORMAT_SUPPORT1_RENDER_TARGET;
		result.Support2 = D3D12_FORMAT_SUPPORT2_NONE;

		return SUCCEEDED(m_device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &result, sizeof(result)));
	}

	//! @brief サポートしているシェーダーステージか 
	bool DirectX12Device::supports(ShaderStage stage)const {
		// TODO サポートするシェーダーステージのチェック
		return true;
	}

	//! @brief  システム・コマンド・キューを取得
	ComPtr<ID3D12CommandQueue>& DirectX12Device::getCommandQueue() {
		return m_commandQueue->getNative();
	}


	//! @brief          ハンドルをアロケート
	//! 
	//! @param type     ヒープタイプ
	//! @param handle   アロケート先ハンドル
	//! @param size     割り当て個数
	void DirectX12Device::allocateHandle(DescriptorHeapType type, DescriptorHandle& handle, s32 size) {
		if (!m_descriptorHeaps.contains(type)) {
			OB_ABORT("不正なDescriptorHeapType");
		}
		m_descriptorHeaps[type]->allocateHandle(handle, size);
	}


	auto DirectX12Device::allocateStagingHandle(DescriptorHeapType type, s32 size) -> D3D12_CPU_DESCRIPTOR_HANDLE {

		if (!m_descriptorStagingHeaps.contains(type)) {
			OB_ABORT("不正なDescriptorHeapType");
		}

		return m_descriptorStagingHeaps[type]->allocate(size);
	}


	//! @brief  SetDescriptorHeaps コマンドを積む
	void DirectX12Device::setDescriptorHeaps(DirectX12CommandList& cmdList) {
		ID3D12DescriptorHeap* pHeaps[] = {
			m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->getNative().Get(),
			m_descriptorHeaps[DescriptorHeapType::Sampler]->getNative().Get(),
		};
		cmdList.getNative()->SetDescriptorHeaps((UINT)std::size(pHeaps), pHeaps);
	}


	//! @brief  IDStorageFactoryを取得
	ComPtr<IDStorageFactory>& DirectX12Device::getDirectStorageFactory() {
		return g_dsfactory;
	}


	//! @brief  DXGIDeviceの初期化
	bool DirectX12Device::initializeDXGIDevice() {
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
			// ID3D12InfoQueue1にQueryInterface
			infoQueue.As(&m_infoQueue);
			// 不必要な警告をフィルター
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
			};
			D3D12_MESSAGE_SEVERITY severities[] = {
			  D3D12_MESSAGE_SEVERITY_INFO,
			};
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(denyIds));
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = static_cast<UINT>(std::size(severities));
			filter.DenyList.pSeverityList = severities;

			infoQueue->PushStorageFilter(&filter);

			// D3D12 エラー発生時にブレーク
			if (m_dx12config.breakWithWarning)infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		}

		OB_DEBUG_CONTEXT(if (!initializeDebugMessageCallback()) return false);

		return true;
	}



	//! @brief  デスクリプタヒープを初期化
	bool DirectX12Device::initializeDescriptorHeaps() {

		// TODO Tierチェック
		// Samplerの割り当て割合調整

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1);
		m_descriptorHeaps[DescriptorHeapType::RTV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::RTV, 256);
		m_descriptorHeaps[DescriptorHeapType::DSV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::DSV, 256);
		m_descriptorHeaps[DescriptorHeapType::Sampler] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::Sampler, D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE);

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->setName("SystemCBV_SRV_UAVHeap");
		m_descriptorHeaps[DescriptorHeapType::RTV]->setName("SystemRTVHeap");
		m_descriptorHeaps[DescriptorHeapType::DSV]->setName("SystemDSVHeap");
		m_descriptorHeaps[DescriptorHeapType::Sampler]->setName("SystemSamplerHeap");


		m_descriptorStagingHeaps[DescriptorHeapType::CBV_SRV_UAV] =
			std::make_unique<DescriptorStagingHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1);
		m_descriptorStagingHeaps[DescriptorHeapType::RTV] =
			std::make_unique<DescriptorStagingHeap>(*this, DescriptorHeapType::RTV, 256);
		m_descriptorStagingHeaps[DescriptorHeapType::DSV] =
			std::make_unique<DescriptorStagingHeap>(*this, DescriptorHeapType::DSV, 256);
		m_descriptorStagingHeaps[DescriptorHeapType::Sampler] =
			std::make_unique<DescriptorStagingHeap>(*this, DescriptorHeapType::Sampler, D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE);

		return true;
	}

	//! @brief  シェーダコンパイラーを初期化
	bool DirectX12Device::initializeShaderCompiler() {

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
	bool DirectX12Device::initializeUploaders() {

		CommandListDesc desc;
		desc.name = "CopyCommandList";
		desc.type = CommandListType::Graphic;
		m_copyCommandList = createCommandList(desc);
					
		size_t blockSize = 4 * 1024;
		m_bufferUploader.construct(*m_device.Get(), blockSize);

		m_textureUploader.construct(*m_device.Get());

		m_descriptorUploader.construct(*m_device.Get());

		return true;
	}

	//! @brief  DirectStorageを初期化
	bool DirectX12Device::initializeDirectStorage() {
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
	void DirectX12Device::clearCommands() {
		m_commandQueue->execute();
		m_commandQueue->wait();
	}

#ifdef OB_DEBUG
	//! @brief  デバッグメッセージコールバックを初期化
	bool DirectX12Device::initializeDebugMessageCallback() {
		if (!m_infoQueue) {
			return true; // InfoQueue1が取得できない場合はスキップ
		}

		// メッセージコールバックを登録
		HRESULT result = m_infoQueue->RegisterMessageCallback(
			debugMessageCallback,
			D3D12_MESSAGE_CALLBACK_FLAG_NONE,
			this, // contextとしてthisポインタを渡す
			&m_callbackCookie
		);

		if (FAILED(result)) {
			LOG_ERROR_EX("DirectX12", "デバッグメッセージコールバックの登録に失敗しました。HRESULT = 0x{:08X}", static_cast<u32>(result));
			return false;
		}

		return true;
	}

	//! @brief  デバッグメッセージコールバックを終了
	void DirectX12Device::finalizeDebugMessageCallback() {
		if (m_infoQueue && m_callbackCookie != 0) {
			m_infoQueue->UnregisterMessageCallback(m_callbackCookie);
			m_callbackCookie = 0;
		}
	}

	//! @brief  デバッグメッセージコールバック関数
	void CALLBACK DirectX12Device::debugMessageCallback(
		D3D12_MESSAGE_CATEGORY category,
		D3D12_MESSAGE_SEVERITY severity,
		D3D12_MESSAGE_ID id,
		LPCSTR description,
		void* context
	) {
		// カテゴリを文字列に変換
		const char* categoryStr = "Unknown";
		switch (category) {
		case D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED: categoryStr = "Application"; break;
		case D3D12_MESSAGE_CATEGORY_MISCELLANEOUS: categoryStr = "Misc"; break;
		case D3D12_MESSAGE_CATEGORY_INITIALIZATION: categoryStr = "Init"; break;
		case D3D12_MESSAGE_CATEGORY_CLEANUP: categoryStr = "Cleanup"; break;
		case D3D12_MESSAGE_CATEGORY_COMPILATION: categoryStr = "Compilation"; break;
		case D3D12_MESSAGE_CATEGORY_STATE_CREATION: categoryStr = "StateCreation"; break;
		case D3D12_MESSAGE_CATEGORY_STATE_SETTING: categoryStr = "StateSetting"; break;
		case D3D12_MESSAGE_CATEGORY_STATE_GETTING: categoryStr = "StateGetting"; break;
		case D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION: categoryStr = "Resource"; break;
		case D3D12_MESSAGE_CATEGORY_EXECUTION: categoryStr = "Execution"; break;
		case D3D12_MESSAGE_CATEGORY_SHADER: categoryStr = "Shader"; break;
		}

		// 重要度に応じてログレベルを決定し、出力
		switch (severity) {
		case D3D12_MESSAGE_SEVERITY_CORRUPTION:
		case D3D12_MESSAGE_SEVERITY_ERROR:
			LOG_ERROR_EX("DirectX12", "[{}] ID:{} {}", categoryStr, static_cast<u32>(id), description);
			CallBreakPoint();
			break;
		case D3D12_MESSAGE_SEVERITY_WARNING:
			LOG_WARNING_EX("DirectX12", "[{}] ID:{} {}", categoryStr, static_cast<u32>(id), description);
			CallBreakPoint();
			break;
		case D3D12_MESSAGE_SEVERITY_INFO:
			LOG_INFO_EX("DirectX12", "[{}] ID:{} {}", categoryStr, static_cast<u32>(id), description);
			break;
		case D3D12_MESSAGE_SEVERITY_MESSAGE:
			LOG_TRACE_EX("DirectX12", "[{}] ID:{} {}", categoryStr, static_cast<u32>(id), description);
			break;
		}
	}
#endif


}
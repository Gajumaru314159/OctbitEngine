//***********************************************************
//! @file
//! @brief		デバイス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DeviceImpl.h"
#include<Plugins/DirectX12RHI/Utility/Utility.h>
#include<Plugins/DirectX12RHI/Display/DisplayImpl.h>
#include<Plugins/DirectX12RHI/Command/CommandListImpl.h>
#include<Plugins/DirectX12RHI/Command/CommandQueue.h>
#include<Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>
#include<Plugins/DirectX12RHI/PipelineState/PipelineStateImpl.h>
#include<Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include<Plugins/DirectX12RHI/RenderPass/RenderPassImpl.h>
#include<Plugins/DirectX12RHI/FrameBuffer/FrameBufferImpl.h>
#include<Plugins/DirectX12RHI/Texture/RenderTextureImpl.h>
#include<Plugins/DirectX12RHI/Shader/ShaderImpl.h>
#include<Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include<Plugins/DirectX12RHI/Descriptor/DescriptorTableImpl.h>
#include<Plugins/DirectX12RHI/Buffer/BufferImpl.h>


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
	DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
		:m_featureLevel(featureLevel) {
		initialize();

		/*


		Ref<RenderPass> renderPass;
		{
			RenderPassDescHelper desc;
			auto color = desc.addAttachment(TextureFormat::RGBA8);
			auto pass0 = desc.addSubpassXCX({ color });

			renderPass = RenderPass::Create(desc);
			OB_ASSERT_EXPR(renderPass);
		}

		Ref<Shader> copyVS;
		Ref<Shader> copyPS;
		{
			String code;
			code.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
			code.append(TC("Texture2D    g_mainTex    :register(t0);						\n"));
			code.append(TC("// IN / OUT														\n"));
			code.append(TC("struct VsIn {													\n"));
			code.append(TC("  float2 pos	:POSITION;										\n"));
			code.append(TC("};																\n"));
			code.append(TC("struct PsIn {													\n"));
			code.append(TC("  float2 pos	:SV_POSITION;									\n"));
			code.append(TC("};																\n"));
			code.append(TC("struct PsOut {													\n"));
			code.append(TC("  float4 color0	:SV_TARGET0;									\n"));
			code.append(TC("};																\n"));
			code.append(TC("// エントリ														\n"));
			code.append(TC("PsIn VS_Main(VsIn i) {											\n"));
			code.append(TC("    PsIn o;														\n"));
			code.append(TC("    o.pos = i.pos;												\n"));
			code.append(TC("    return o;													\n"));
			code.append(TC("}																\n"));
			code.append(TC("PsOut PS_Main(PsIn i){											\n"));
			code.append(TC("    PsOut o;													\n"));
			code.append(TC("    o.color0 = g_mainTex.Sample(g_mainSampler,i.pos *0.5 + 0.5);\n"));
			code.append(TC("    return o;													\n"));
			code.append(TC("}																\n"));

			copyVS = VertexShader::Create(code);
			copyPS = PixelShader::Create(code);
			OB_ASSERT_EXPR(copyVS && copyPS);
		}

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc(
				{
					RootParameter::Range(DescriptorRangeType::SRV,1,0)
				},
				{
					StaticSamplerDesc(SamplerDesc(),0),
				}
			);
			signature = RootSignature::Create(desc);
			signature->setName(TC("CopyRootSignature"));
			OB_ASSERT_EXPR(signature);
		}

		Ref<PipelineState> pipeline;
		{
			PipelineStateDesc desc;
			desc.renderPass = renderPass;
			desc.subpass = 0;

			desc.rootSignature = signature;
			desc.vs = copyVS;
			desc.ps = copyPS;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,0.0f,Type::Float,2),
			};
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
			desc.depthStencil.stencil.enable = false;

			pipeline = PipelineState::Create(desc);
			pipeline->setName(TC("TestPipeline"));
			OB_ASSERT_EXPR(pipeline);
		}
		*/

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
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーパスを生成を生成
	//@―---------------------------------------------------------------------------
	Ref<RenderPass> DeviceImpl::createRenderPass(const RenderPassDesc& desc) {
		SAFE_CREATE(RenderPassImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  フレームバッファを生成
	//@―---------------------------------------------------------------------------
	Ref<FrameBuffer> DeviceImpl::createFrameBuffer(const FrameBufferDesc& desc) {
		SAFE_CREATE(FrameBufferImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  スワップ・チェーンを生成
	//@―---------------------------------------------------------------------------
	Ref<Display> DeviceImpl::createDisplay(const DisplayDesc& desc) {
		SAFE_CREATE(DisplayImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドリストを生成
	//@―---------------------------------------------------------------------------
	Ref<CommandList> DeviceImpl::createCommandList(const CommandListDesc& desc) {
		SAFE_CREATE(CommandListImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ルートシグネチャを生成
	//@―---------------------------------------------------------------------------
	Ref<RootSignature> DeviceImpl::createRootSignature(const RootSignatureDesc& desc) {
		SAFE_CREATE(RootSignatureImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステートを生成
	//@―---------------------------------------------------------------------------
	Ref<PipelineState> DeviceImpl::createPipelineState(const PipelineStateDesc& desc) {
		SAFE_CREATE(PipelineStateImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<Texture> DeviceImpl::createTexture(const TextureDesc& desc) {
		SAFE_CREATE(TextureImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<Texture> DeviceImpl::createTexture(BlobView blob) {
		SAFE_CREATE(TextureImpl,*this, blob);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーテクスチャを生成
	//@―---------------------------------------------------------------------------
	Ref<RenderTexture> DeviceImpl::createRenderTexture(const RenderTextureDesc& desc) {
		SAFE_CREATE(RenderTextureImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バッファーを生成
	//@―---------------------------------------------------------------------------
	Ref<Buffer> DeviceImpl::createBuffer(const BufferDesc& desc) {
		SAFE_CREATE(BufferImpl,*this, desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	Ref<Shader> DeviceImpl::createShader(const String& code, ShaderStage stage) {
		SAFE_CREATE(ShaderImpl,code, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	Ref<Shader> DeviceImpl::createShader(const Blob& binary, ShaderStage stage) {
		SAFE_CREATE(ShaderImpl,binary, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブルを生成
	//@―---------------------------------------------------------------------------
	Ref<DescriptorTable> DeviceImpl::createDescriptorTable(DescriptorHeapType type, s32 elementNum) {
		if (m_descriptorHeaps.find(type)==m_descriptorHeaps.end())return nullptr;
		SAFE_CREATE(DescriptorTableImpl,*m_descriptorHeaps[type], type, elementNum);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  システム・コマンド・キューを取得
	//@―---------------------------------------------------------------------------
	ComPtr<ID3D12CommandQueue>& DeviceImpl::getCommandQueue() {
		return m_commandQueue->getNative();
	}


	//@―---------------------------------------------------------------------------
	//! @brief          ハンドルをアロケート
	//! 
	//! @param type     ヒープタイプ
	//! @param handle   アロケート先ハンドル
	//! @param size     割り当て個数
	//@―---------------------------------------------------------------------------
	void DeviceImpl::allocateHandle(DescriptorHeapType type, DescriptorHandle& handle, s32 size) {
		if (m_descriptorHeaps.find(type) == m_descriptorHeaps.end()) {
			OB_ABORT("不正なDescriptorHeapType");
		}
		m_descriptorHeaps[type]->allocateHandle(handle, size);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  SetDescriptorHeaps コマンドを積む
	//@―---------------------------------------------------------------------------
	void DeviceImpl::setDescriptorHeaps(CommandListImpl& cmdList) {
		ID3D12DescriptorHeap* pHeaps[] = {
			m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->getNative().Get(),
			m_descriptorHeaps[DescriptorHeapType::Sampler]->getNative().Get(),
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
	bool DeviceImpl::initializeVideoCardInfo() {
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタヒープを初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initializeDescriptorHeaps() {

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::CBV_SRV_UAV, 10'000);
		m_descriptorHeaps[DescriptorHeapType::Sampler] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::Sampler, 100);
		m_descriptorHeaps[DescriptorHeapType::RTV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::RTV, 100);
		m_descriptorHeaps[DescriptorHeapType::DSV] =
			std::make_unique<DescriptorHeap>(*this, DescriptorHeapType::DSV, 100);

		m_descriptorHeaps[DescriptorHeapType::CBV_SRV_UAV]->setName(TC("SystemCBV_SRV_UAVHeap"));
		m_descriptorHeaps[DescriptorHeapType::Sampler]->setName(TC("SystemSamplerHeap"));
		m_descriptorHeaps[DescriptorHeapType::RTV]->setName(TC("SystemRTVHeap"));
		m_descriptorHeaps[DescriptorHeapType::DSV]->setName(TC("SystemDSVHeap"));

		return true;
	}

}// namespace ob::rhi::dx12
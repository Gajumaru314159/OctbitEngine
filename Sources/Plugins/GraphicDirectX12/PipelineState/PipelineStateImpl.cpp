//***********************************************************
//! @file
//! @brief		パイプラインステート実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/GraphicDirectX12/PipelineState/PipelineStateImpl.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/RootSignature/RootSignatureImpl.h>
#include <Plugins/GraphicDirectX12/Shader/ShaderImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>
#include <Framework/Graphic/Private/Device.h>

namespace ob::graphic::dx12 {


	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineStateImpl::PipelineStateImpl(DeviceImpl& rDevice, const PipelineStateDesc& desc)
		: m_desc(desc)
	{

		if (!desc.rootSignature) {
			LOG_ERROR_EX("Graphic", "パイプラインステートの構築に失敗。RootSignatureが設定されていません。");
			return;
		}
		if (!desc.target) {
			LOG_ERROR_EX("Graphic", "パイプラインステートの構築に失敗。RenderTargetが設定されていません。");
			return;
		}

		auto vs = Device::GetImpl<ShaderImpl>(m_desc.vs);
		auto ps = Device::GetImpl<ShaderImpl>(m_desc.ps);
		auto gs = Device::GetImpl<ShaderImpl>(m_desc.gs);
		auto hs = Device::GetImpl<ShaderImpl>(m_desc.hs);
		auto ds = Device::GetImpl<ShaderImpl>(m_desc.ds);

		if (vs == nullptr) {
			LOG_ERROR_EX("Graphic", "パイプラインステートの構築に失敗。頂点シェーダが設定されていません。");
			return;
		}
		if (ps == nullptr) {
			LOG_ERROR_EX("Graphic", "パイプラインステートの構築に失敗。ピクセルシェーダが設定されていません。");
			return;
		}

		const auto targetCount = desc.target.getColorTextureCount();
		if (8 < targetCount) {
			LOG_ERROR_EX("Graphic", "PipelineStateの構築に失敗。描画先枚数が8以上です。[num={}]", targetCount);
			return;
		}

		// グラフィックパイプラインの定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};

		gpsd.pRootSignature = Device::GetImpl<RootSignatureImpl>(desc.rootSignature)->getNative().Get();

		// シェーダ
		{
			if (vs)gpsd.VS = CD3DX12_SHADER_BYTECODE(vs->getBinaryData(), vs->getBinarySize());
			if (vs)gpsd.PS = CD3DX12_SHADER_BYTECODE(ps->getBinaryData(), ps->getBinarySize());
			if (gs)gpsd.PS = CD3DX12_SHADER_BYTECODE(gs->getBinaryData(), gs->getBinarySize());
			if (hs)gpsd.PS = CD3DX12_SHADER_BYTECODE(hs->getBinaryData(), hs->getBinarySize());
			if (ds)gpsd.PS = CD3DX12_SHADER_BYTECODE(ds->getBinaryData(), ds->getBinarySize());
		}

		// TODO: StreamOutput
		{

		}

		// レンダーターゲットごとの設定
		{
			// フォーマット
			gpsd.NumRenderTargets = targetCount;
			for (s32 i = 0; i < targetCount; ++i) {
				auto& tex = desc.target.getColorTexture(i);
				gpsd.RTVFormats[i] = TypeConverter::convert(tex.format());
			}
			auto depthTex = desc.target.getDepthTexture();
			if (depthTex)gpsd.DSVFormat = TypeConverter::convert(depthTex.format());
		}

		setupBlend(gpsd.BlendState, desc);
		setupRasterizerState(gpsd.RasterizerState, desc.rasterizer);
		setupDepthStencilState(gpsd.DepthStencilState, desc.depthStencil);

		// 頂点レイアウト
		vector<D3D12_INPUT_ELEMENT_DESC> attributes;
		gpsd.InputLayout.NumElements = gsl::narrow_cast<UINT>(desc.vertexLayout.attributes.size());
		attributes.reserve(gpsd.InputLayout.NumElements);
		for (auto& attr : desc.vertexLayout.attributes) {
			attributes.push_back({});
			setupVertexLayout(attributes.back(), attr);
		}
		gpsd.InputLayout.pInputElementDescs = attributes.data();

		gpsd.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		gpsd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;// TypeConverter::convert(desc.topology);
		gpsd.SampleDesc.Count = desc.sample.count;
		gpsd.SampleDesc.Quality = desc.sample.qualitty;
		gpsd.NodeMask = 0;
		gpsd.SampleMask = desc.sampleMask;


		// グラフィックパイプラインを生成
		HRESULT result;
		ComPtr<ID3D12PipelineState> pipelineState;
		result = rDevice.getNativeDevice()->CreateGraphicsPipelineState(&gpsd, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("ID3D12Device::CreateGraphicsPipelineState()"));
			return;
		}

		// リソースを参照に追加
		m_pipelineState = pipelineState;
		m_rootSignature = m_rootSignature;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ブレンド設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupBlend(D3D12_BLEND_DESC& dst, const PipelineStateDesc& src) {
		dst = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		for (s32 i = 0; i < get_size(src.blend); ++i) {
			setupRenderTargetBlend(dst.RenderTarget[i], src.blend[i]);
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーターゲットごとのブレンド設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupRenderTargetBlend(D3D12_RENDER_TARGET_BLEND_DESC& dst, const BlendDesc& src) {
		dst.BlendEnable = src.blendEnable;
		dst.SrcBlend = TypeConverter::convert(src.srcColorFactor);
		dst.DestBlend = TypeConverter::convert(src.dstColorFactor);
		dst.BlendOp = TypeConverter::convert(src.colorOp);
		dst.SrcBlendAlpha = TypeConverter::convert(src.srcAlphaFactor);
		dst.DestBlendAlpha = TypeConverter::convert(src.dstAlphaFactor);
		dst.BlendOpAlpha = TypeConverter::convert(src.alphaOp);
		dst.RenderTargetWriteMask = TypeConverter::convert(src.mask);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点レイアウト設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupVertexLayout(D3D12_INPUT_ELEMENT_DESC& dst, const VertexAttribute&src) {
		dst.SemanticName = TypeConverter::convert(src.semantic);
		dst.SemanticIndex = src.index;
		dst.Format = TypeConverter::convert(src.type, src.dimention);
		dst.InputSlot = 0;
		dst.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		dst.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		dst.InstanceDataStepRate = 0;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ラスタライズ設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupRasterizerState(D3D12_RASTERIZER_DESC& dst, const RasterizerDesc& src) {

		dst = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		dst.FillMode = TypeConverter::convert(src.fillMode);
		dst.CullMode = TypeConverter::convert(src.cullMode);
		//FrontCounterClockwise = FALSE;
		//DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		//DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		//SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		//DepthClipEnable = TRUE;
		//MultisampleEnable = FALSE;
		//AntialiasedLineEnable = FALSE;
		//ForcedSampleCount = 0;
		//ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デプス・ステンシル設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupDepthStencilState(D3D12_DEPTH_STENCIL_DESC& dst, const DepthStencilDesc& src) {
		dst = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		dst.DepthEnable=src.depth.enable;
		dst.DepthWriteMask=src.depth.write? D3D12_DEPTH_WRITE_MASK_ALL:D3D12_DEPTH_WRITE_MASK_ZERO;
		dst.DepthFunc=TypeConverter::convert(src.depth.func);
		
		dst.StencilEnable=src.stencil.enable;
		dst.StencilReadMask=src.stencil.read_mask;
		dst.StencilWriteMask=src.stencil.write_mask;
		
		dst.FrontFace.StencilFailOp = TypeConverter::convert(src.stencil.failOp);
		dst.FrontFace.StencilDepthFailOp = TypeConverter::convert(src.stencil.depthFailOp);
		dst.FrontFace.StencilPassOp = TypeConverter::convert(src.stencil.passOp);
		dst.FrontFace.StencilFunc= TypeConverter::convert(src.stencil.func);
		dst.BackFace= dst.FrontFace;
	}


}// namespace ob::graphic::dx12
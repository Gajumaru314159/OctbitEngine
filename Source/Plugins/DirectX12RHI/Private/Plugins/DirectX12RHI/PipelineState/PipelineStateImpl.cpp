//***********************************************************
//! @file
//! @brief		パイプラインステート実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/DirectX12RHI/Shader/ShaderImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi::dx12 {


	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	PipelineStateImpl::PipelineStateImpl(DeviceImpl& rDevice, const PipelineStateDesc& desc)
		: m_desc(desc)
	{

		if (!desc.rootSignature) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。RootSignatureが設定されていません。");
			return;
		}
		if (!desc.renderPass) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。RenderPassが設定されていません。");
			return;
		}

		if (!m_desc.vs) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。頂点シェーダが設定されていません。");
			return;
		}
		if (!m_desc.ps) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。ピクセルシェーダが設定されていません。");
			return;
		}

		if (!desc.renderPass) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。RenderPassが設定されていません。");
		}

		if (!is_in_range(desc.subpass, desc.renderPass->desc().subpasses)) {
			LOG_FATAL_EX("Graphic", "パイプラインステートの構築に失敗。サブパスが範囲外です。[index={},max={}]", desc.subpass, desc.renderPass->desc().subpasses.size());
		}

		auto& subpass = desc.renderPass->desc().subpasses[desc.subpass];
		const auto targetCount = subpass.colors.size();
		if (8 < targetCount) {
			LOG_FATAL_EX("Graphic", "PipelineStateの構築に失敗。描画先枚数が8以上です。[num={}]", targetCount);
			return;
		}

		// グラフィックパイプラインの定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};


		// シェーダ
		{
			if (m_desc.vs) {
				auto& shader = *m_desc.vs.cast<ShaderImpl>();
				gpsd.VS = CD3DX12_SHADER_BYTECODE(shader.getBinaryData(), shader.getBinarySize());
			}
			//if (m_desc.gs) {
			//	auto& shader = *m_desc.vs.cast<ShaderImpl>();
			//	gpsd.GS = CD3DX12_SHADER_BYTECODE(shader.getBinaryData(), shader.getBinarySize());
			//}
			//if (m_desc.hs) {
			//	auto& shader = *m_desc.vs.cast<ShaderImpl>();
			//	gpsd.HS = CD3DX12_SHADER_BYTECODE(shader.getBinaryData(), shader.getBinarySize());
			//}
			//if (m_desc.ds) {
			//	auto& shader = *m_desc.vs.cast<ShaderImpl>();
			//	gpsd.PS = CD3DX12_SHADER_BYTECODE(shader.getBinaryData(), shader.getBinarySize());
			//}
			if (m_desc.ps) {
				auto& shader = *m_desc.ps.cast<ShaderImpl>();
				gpsd.PS = CD3DX12_SHADER_BYTECODE(shader.getBinaryData(), shader.getBinarySize());
			}
		}

		// TODO: StreamOutput
		{

		}

		setupFormats(gpsd, desc);
		setupBlend(gpsd.BlendState, desc);
		setupRasterizerState(gpsd.RasterizerState, desc.rasterizer);
		setupDepthStencilState(gpsd.DepthStencilState, desc.depthStencil);

		// 頂点レイアウト
		Array<D3D12_INPUT_ELEMENT_DESC> attributes;
		gpsd.InputLayout.NumElements = gsl::narrow_cast<UINT>(desc.vertexLayout.attributes.size());
		attributes.reserve(gpsd.InputLayout.NumElements);
		for (auto& attr : desc.vertexLayout.attributes) {
			attributes.push_back({});
			setupVertexLayout(attributes.back(), attr);
		}
		gpsd.InputLayout.pInputElementDescs = attributes.data();

		gpsd.pRootSignature = desc.rootSignature.cast<RootSignatureImpl>()->getNative();
		gpsd.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		gpsd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;// TypeConverter::Convert(desc.topology);
		gpsd.SampleDesc.Count = desc.sample.count;
		gpsd.SampleDesc.Quality = desc.sample.qualitty;
		gpsd.NodeMask = 0;
		gpsd.SampleMask = desc.sampleMask;


		// グラフィックパイプラインを生成
		HRESULT result;
		ComPtr<ID3D12PipelineState> pipelineState;
		result = rDevice.getNative()->CreateGraphicsPipelineState(&gpsd, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateGraphicsPipelineState()"));
			return;
		}

		// リソースを参照に追加
		m_pipelineState = pipelineState;
		Utility::setName(m_pipelineState.Get(), getName());
	}


	//@―---------------------------------------------------------------------------
	//! @brief  フォーマット設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupFormats(D3D12_GRAPHICS_PIPELINE_STATE_DESC& dst, const PipelineStateDesc& src) {

		auto& subpass = src.renderPass->desc().subpasses[src.subpass];
		auto& attachments = src.renderPass->desc().attachments;

		dst.NumRenderTargets = (UINT)subpass.colors.size();

		for (auto [i, ref] : Indexed(subpass.colors)) {

			auto index = ref.index;

			if (is_in_range(index, attachments)) {
				auto format = TypeConverter::Convert(attachments[index].format);
				dst.RTVFormats[i] = format;
			} else {
				LOG_ERROR("無効なカラーアタッチメントインデックス[index={},max={}]", index, attachments.size());
			}

		}

		if (!subpass.depth.empty()) {

			auto index = subpass.depth[0].index;

			if (is_in_range(index, attachments)) {
				auto format = TypeConverter::Convert(attachments[index].format);
				dst.DSVFormat = format;
			} else {
				LOG_ERROR("無効なデプスアタッチメントインデックス[index={},max={}]", index, attachments.size());
			}

		}

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ブレンド設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupBlend(D3D12_BLEND_DESC& dst, const PipelineStateDesc& src) {
		dst = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		for (s32 i = 0; i < std::size(src.blend); ++i) {
			setupRenderTargetBlend(dst.RenderTarget[i], src.blend[i]);
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーターゲットごとのブレンド設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupRenderTargetBlend(D3D12_RENDER_TARGET_BLEND_DESC& dst, const BlendDesc& src) {
		dst.BlendEnable = src.blendEnable;
		dst.SrcBlend = TypeConverter::Convert(src.srcColorFactor);
		dst.DestBlend = TypeConverter::Convert(src.dstColorFactor);
		dst.BlendOp = TypeConverter::Convert(src.colorOp);
		dst.SrcBlendAlpha = TypeConverter::Convert(src.srcAlphaFactor);
		dst.DestBlendAlpha = TypeConverter::Convert(src.dstAlphaFactor);
		dst.BlendOpAlpha = TypeConverter::Convert(src.alphaOp);
		dst.RenderTargetWriteMask = TypeConverter::Convert(src.mask);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点レイアウト設定
	//@―---------------------------------------------------------------------------
	void PipelineStateImpl::setupVertexLayout(D3D12_INPUT_ELEMENT_DESC& dst, const VertexAttribute& src) {
		dst.SemanticName = TypeConverter::Convert(src.semantic);
		dst.SemanticIndex = src.index;
		dst.Format = TypeConverter::Convert(src.type, src.dimention);
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
		dst.FillMode = TypeConverter::Convert(src.fillMode);
		dst.CullMode = TypeConverter::Convert(src.cullMode);
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
		dst.DepthEnable = src.depth.enable;
		dst.DepthWriteMask = src.depth.write ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
		dst.DepthFunc = TypeConverter::Convert(src.depth.func);

		dst.StencilEnable = src.stencil.enable;
		dst.StencilReadMask = src.stencil.read_mask;
		dst.StencilWriteMask = src.stencil.write_mask;

		dst.FrontFace.StencilFailOp = TypeConverter::Convert(src.stencil.failOp);
		dst.FrontFace.StencilDepthFailOp = TypeConverter::Convert(src.stencil.depthFailOp);
		dst.FrontFace.StencilPassOp = TypeConverter::Convert(src.stencil.passOp);
		dst.FrontFace.StencilFunc = TypeConverter::Convert(src.stencil.func);
		dst.BackFace = dst.FrontFace;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      名前を取得
	//@―---------------------------------------------------------------------------
	const String& PipelineStateImpl::getName()const {
		return m_desc.name;
	}

}// namespace ob::rhi::dx12
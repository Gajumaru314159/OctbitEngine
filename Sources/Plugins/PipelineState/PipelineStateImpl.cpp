//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/GraphicDirectX12/Pipeline/GraphicPipelineImpl.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Shader/ShaderImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Framework/Graphic/Private/Device.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief		コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicPipelineImpl::GraphicPipelineImpl(DeviceImpl& rDevice,const GraphicPipelineDesc& desc) {
		m_desc = desc;
		auto vs = Device::GetImpl<ShaderImpl>(m_desc.vs);
		auto ps = Device::GetImpl<ShaderImpl>(m_desc.ps);

		if (vs == nullptr)return;
		if (ps == nullptr)return;

		for (s32 i = 0; i < REGISTER_MAX; i++) {
			m_cBufDataSizes[i] = 0;
			m_texNums[i] = 0;
			m_cBufIndices[i] = -1;
			m_texIndices[i] = -1;
		}


		// 頂点レイアウトを取得
		if (reflectInputLayout(vs->getShaderBolb()) == -1)return;
		if (reflectOutputLayout(ps->getShaderBolb()) == -1)return;

		// 定数バッファのリフレクション
		if (vs != nullptr)reflectShader(vs->getShaderBolb()); else return;
		if (ps != nullptr)reflectShader(ps->getShaderBolb()); else return;



		// ディスクリプタレンジの生成
		vector<CD3DX12_DESCRIPTOR_RANGE> descriptorRanges;

		// 定数バッファ
		for (s32 i = 0; i < REGISTER_MAX; i++) {
			if (m_cBufDataSizes[i] == 0)continue;
			m_cBufIndices[i] = descriptorRanges.size();
			CD3DX12_DESCRIPTOR_RANGE dr;
			dr.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i);
			descriptorRanges.push_back(dr);
		}
		// テクスチャ
		for (s32 i = 0; i < REGISTER_MAX; i++) {
			if (m_texNums[i] == 0)continue;
			m_texIndices[i] = descriptorRanges.size();
			CD3DX12_DESCRIPTOR_RANGE dr;
			dr.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, m_texNums[i], i);
			descriptorRanges.push_back(dr);
		}



		// SetDescriptorHeapsで割り当てる塊
		CD3DX12_ROOT_PARAMETER rootParams[REGISTER_MAX + REGISTER_MAX] = {};
		for (s32 i = 0; i < descriptorRanges.size(); i++) {
			rootParams[i].InitAsDescriptorTable(1, &descriptorRanges[i]);
		}


		// 静的サンプラー
		// TODO テクスチャごとのサンプラーを定義するか検討
		CD3DX12_STATIC_SAMPLER_DESC samplerDescs[1] = {};
		samplerDescs[0].Init(0);




		// ルートシグネチャディスクリプタ
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Init((UINT)descriptorRanges.size(), rootParams, 1, samplerDescs, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		// バイナリデータの作成
		ComPtr<ID3DBlob> rootSigBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		if (FAILED(result)) {
			return;
		}

		// ルートシグネチャの作成
		result = rDevice.getNativeDevice()->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			return;
		}


		// グラフィックパイプラインの定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};

		if (vs)pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vs->getShaderBolb().Get());
		if (ps)pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(ps->getShaderBolb().Get());
		//if (gs)pipelineStateDesc.GS = CD3DX12_SHADER_BYTECODE(gs->getShaderBolb().Get());
		//if (hs)pipelineStateDesc.HS = CD3DX12_SHADER_BYTECODE(hs->getShaderBolb().Get());
		//if (ds)pipelineStateDesc.DS = CD3DX12_SHADER_BYTECODE(ds->getShaderBolb().Get());

		// TODO: StreamOutput

		// TODO: ブレンドモード指定
		pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		for (s32 i = 0; i < 8; i++) {
			pipelineStateDesc.BlendState.RenderTarget[i].BlendEnable = true;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlend = ms_blendFactorMap.at(desc.blendMode[i].src);
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlend = ms_blendFactorMap.at(desc.blendMode[i].dest);
		}

		pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		if (desc.useWireframe)pipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
		else pipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		pipelineStateDesc.RasterizerState.MultisampleEnable = desc.useMultisample;

		switch (desc.cullMode) {
		case CullMode::None:pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; break;
		case CullMode::Front:pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT; break;
		case CullMode::Back:pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; break;
		}

		// TODO 深度バッファ
		pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pipelineStateDesc.DepthStencilState.DepthEnable = desc.zTest;
		pipelineStateDesc.DepthStencilState.StencilEnable = desc.useStencil;
		pipelineStateDesc.DepthStencilState.DepthWriteMask = desc.zWrite ?
			D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ZERO;

		pipelineStateDesc.InputLayout.pInputElementDescs = m_inputLayout.data();			//頂点レイアウト先頭アドレス
		pipelineStateDesc.InputLayout.NumElements = (UINT)m_inputLayout.size();				//頂点レイアウトサイズ

		switch (desc.primitiveTopologyType) {
		case PrimitiveTopology::Point:pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		case PrimitiveTopology::Line:pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case PrimitiveTopology::Triangle:pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}

		pipelineStateDesc.NumRenderTargets = desc.numRenderTargets;							//レンダ―ターゲットの数
		for (s32 i = 0; i < pipelineStateDesc.NumRenderTargets && i < 8; i++) {
			pipelineStateDesc.RTVFormats[i] = Utility::convertTextureFormat(desc.textureFormats[i]);					//0～1に正規化されたRGBA
		}

		pipelineStateDesc.SampleDesc.Count = 1;  //サンプリングは1ピクセルにつき１
		pipelineStateDesc.SampleDesc.Quality = 0;  //クオリティは最低


		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;




		pipelineStateDesc.pRootSignature = m_rootSignature.Get();



		// グラフィックパイプラインの生成
		ComPtr<ID3D12PipelineState> pipelineState;
		result = rDevice.getNativeDevice()->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			return;
		}


		// リソースを参照に追加
		m_pipelineState = pipelineState;
		m_vs = desc.vs;
		m_ps = desc.ps;
    }


	//s32 GraphicPipelineImpl::setGraphicPipeline(ComPtr<ID3D12GraphicsCommandList>& commandList)const {
	//	if (!IsValid())return -1;
	//	if (!commandList)return -1;
	//	commandList->SetPipelineState(m_pipelineState.Get());
	//	commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	//	return 0;
	//}


	s32 GraphicPipelineImpl::getConstantBufferSize(const u32 resister)const {
		if (resister < 0 || MAX_REGISTER <= resister)return -1;
		return (m_cBufDataSizes[resister] + 0xff) & ~0xff;
	}

	s32 GraphicPipelineImpl::getTextureNum(const u32 resister)const {
		if (resister < 0 || MAX_REGISTER <= resister)return -1;
		return m_texNums[resister];
	}

	s32 GraphicPipelineImpl::getConstantBufferIndex(const u32 resister)const {
		if (resister < 0 || MAX_REGISTER <= resister)return -1;
		return m_cBufIndices[resister];
	}
	s32 GraphicPipelineImpl::getTextureIndex(const u32 resister)const {
		if (resister < 0 || MAX_REGISTER <= resister)return -1;
		return m_texIndices[resister];
	}


	ShaderVariableDesc GraphicPipelineImpl::getVariableData(const String& name)const {
		if (m_varMap.count(name) == 0) {
			ShaderVariableDesc desc = {};
			desc.registerNum = -1;
			return desc;
		}
		return m_varMap.at(name);
	}

	ShaderTextureDesc GraphicPipelineImpl::getTextureData(const String& name)const {
		if (m_texMap.count(name) == 0) {
			ShaderTextureDesc desc = {};
			desc.registerNum = -1;
			return desc;
		}
		return m_texMap.at(name);
	}


	const GraphicPipelineImpl::map_type& GraphicPipelineImpl::getShaderParamList()const {
		static const HashMap<String, ShaderVariableDesc> emptyMap;
		if (!IsValid())return emptyMap;
		return m_varMap;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		ID3DBlob からシェーダ情報を取得
	//@―---------------------------------------------------------------------------
	s32 GraphicPipelineImpl::reflectShader(const ComPtr<ID3DBlob>& shaderBolb) {
		if (!shaderBolb)return -1;

		ComPtr<ID3D12ShaderReflection> reflection;
		D3DReflect(
			shaderBolb->GetBufferPointer(),
			shaderBolb->GetBufferSize(),
			IID_ID3D12ShaderReflection, (void**)reflection.ReleaseAndGetAddressOf());

		reflectConstantBuffer(reflection);
		reflectBoundingResource(reflection);

		return 0;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		ID3DBlob から頂点レイアウトを取得
	//@―---------------------------------------------------------------------------
	s32 GraphicPipelineImpl::reflectInputLayout(const ComPtr<ID3DBlob>& vsInstance) {
		if (!vsInstance)return -1;

		ComPtr<ID3D12ShaderReflection> reflection;
		D3DReflect(vsInstance->GetBufferPointer(), vsInstance->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)reflection.ReleaseAndGetAddressOf());

		if (!reflection)return -1;

		D3D12_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		m_inputLayoutNames.resize(shaderDesc.InputParameters);
		for (u32 i = 0; i < shaderDesc.InputParameters; i++) {
			D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
			reflection->GetInputParameterDesc(i, &paramDesc);

			m_inputLayoutNames[i] = paramDesc.SemanticName;

			D3D12_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = m_inputLayoutNames[i].c_str();
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			if (paramDesc.Mask == 1) {
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			} else if (paramDesc.Mask <= 3) {
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			} else if (paramDesc.Mask <= 7) {
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			} else if (paramDesc.Mask <= 15) {
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			if (m_inputLayoutNames[i] == TC("POSITION")) {
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}

			m_inputLayout.push_back(elementDesc);
		}

		return 0;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		ID3D12ShaderReflection から変数情報を取得
	//@―---------------------------------------------------------------------------
	s32 GraphicPipelineImpl::reflectConstantBuffer(const ComPtr<ID3D12ShaderReflection>& reflection) {
		if (!reflection)return -1;

		D3D12_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		for (u32 cBufIdx = 0; cBufIdx < shaderDesc.ConstantBuffers; cBufIdx++) {
			auto cbufferPtr = reflection->GetConstantBufferByIndex(cBufIdx);
			if (cbufferPtr == nullptr)return -1;

			D3D12_SHADER_BUFFER_DESC bufferDesc;
			cbufferPtr->GetDesc(&bufferDesc);

			D3D12_SHADER_INPUT_BIND_DESC inputDesc;
			reflection->GetResourceBindingDescByName(bufferDesc.Name, &inputDesc);


			// 定数バッファのサイズがより大きければ更新
			u32 registerNum = inputDesc.BindPoint;
			if (m_cBufDataSizes[registerNum] < bufferDesc.Size) {
				m_cBufDataSizes[registerNum] = bufferDesc.Size;
			}


			for (UINT varIdx = 0; varIdx < bufferDesc.Variables; varIdx++) {
				auto variable = cbufferPtr->GetVariableByIndex(varIdx);
				if (variable == nullptr)continue;

				auto reflectionType = variable->GetType();

				D3D12_SHADER_TYPE_DESC typeDesc;
				reflectionType->GetDesc(&typeDesc);

				D3D12_SHADER_VARIABLE_DESC variableDesc;
				variable->GetDesc(&variableDesc);


				ShaderVariableDesc vdesc;
				vdesc.offset = variableDesc.StartOffset;
				vdesc.registerNum = registerNum;

				vdesc.type = typeDesc.Type;
				vdesc.rows = typeDesc.Rows;
				vdesc.columns = typeDesc.Columns;
				vdesc.elementCount = typeDesc.Elements;
				if (vdesc.elementCount == 0)vdesc.elementCount = 1;

				m_varMap[variableDesc.Name] = vdesc;
			}
		}
		return 0;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		ID3D12ShaderReflection からテクスチャ情報を取得
	//@―---------------------------------------------------------------------------
	s32 GraphicPipelineImpl::reflectBoundingResource(const ComPtr<ID3D12ShaderReflection>& reflection) {
		if (!reflection)return -1;

		D3D12_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		for (u32 i = 0; i < shaderDesc.BoundResources; i++) {
			D3D12_SHADER_INPUT_BIND_DESC desc;
			reflection->GetResourceBindingDesc(i, &desc);
			if (desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE) {
				ShaderTextureDesc tdesc;
				tdesc.registerNum = desc.BindPoint;
				if (desc.Dimension == D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2D)tdesc.dimension = 2;
				if (desc.Dimension == D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2DARRAY)tdesc.dimension = 2;
				if (desc.Dimension == D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE3D)tdesc.dimension = 3;

				m_texMap[desc.Name] = tdesc;

				if (m_texNums[desc.BindPoint] < desc.BindCount) {
					m_texNums[desc.BindPoint] = desc.BindCount;
				}
			}
		}
		return 0;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		ID3DBlob からアウトプットするテクスチャ枚数を取得
	//@―---------------------------------------------------------------------------
	s32 GraphicPipelineImpl::reflectOutputLayout(const ComPtr<ID3DBlob>& vsInstance) {
		if (!vsInstance)return -1;

		ComPtr<ID3D12ShaderReflection> reflection;
		D3DReflect(vsInstance->GetBufferPointer(), vsInstance->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)reflection.ReleaseAndGetAddressOf());

		if (!reflection)return -1;

		D3D12_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);
		m_targetNum = shaderDesc.OutputParameters;
		if (m_targetNum <= 0 || 8 <= shaderDesc.OutputParameters)return -1;
		return 0;
	}

}// namespace ob::graphic::dx12
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/PipelineState/DirectX12ComputePipelineState.h>
#include <Plugins/DirectX12RHI/DirectX12Device.h>
#include <Plugins/DirectX12RHI/RootSignature/DirectX12RootSignature.h>
#include <Plugins/DirectX12RHI/Shader/DirectX12Shader.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>

namespace ob::rhi {


	//! @brief		コンストラクタ
	DirectX12ComputePipelineState::DirectX12ComputePipelineState(DirectX12Device& device, const ComputePipelineStateDesc& desc)
		: m_desc(desc)
	{

		if (!desc.rootSignature) {
			LOG_ERROR_EX("Graphic", "コンピュートパイプラインステートの構築に失敗。RootSignatureが設定されていません。");
			return;
		}

		if (!m_desc.cs) {
			LOG_ERROR_EX("Graphic", "コンピュートパイプラインステートの構築に失敗。コンピュートシェーダが設定されていません。");
			return;
		}

		// コンピュートパイプラインの定義
		D3D12_COMPUTE_PIPELINE_STATE_DESC cpsd = {};

		// コンピュートシェーダ
		if (m_desc.cs) {
			auto& shader = *m_desc.cs.cast<DirectX12Shader>();
			cpsd.CS = CD3DX12_SHADER_BYTECODE(shader.getBinaryData(), shader.getBinarySize());
		}

		cpsd.pRootSignature = desc.rootSignature.cast<DirectX12RootSignature>()->getNative();
		cpsd.NodeMask = 0;

		// コンピュートパイプラインを生成
		HRESULT result;
		ComPtr<ID3D12PipelineState> pipelineState;
		result = device.getNative()->CreateComputePipelineState(&cpsd, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "ID3D12Device::CreateComputePipelineState()");
			return;
		}

		// リソースを参照に追加
		m_pipelineState = pipelineState;
		Utility::SetName(m_pipelineState.Get(), m_desc.name);

		manage();
	}


	//! @brief      名前を取得
	const String& DirectX12ComputePipelineState::getName()const {
		return m_desc.name;
	}

}
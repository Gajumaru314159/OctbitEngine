//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "RootSignatureImpl.h"
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RootSignatureImpl::RootSignatureImpl(DeviceImpl& rDevice, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		// パラメータ
		Array<D3D12_ROOT_PARAMETER> parameters;
		Array<CD3DX12_DESCRIPTOR_RANGE> descriptors;
		parameters.reserve(desc.parameters.size());
		descriptors.reserve(desc.parameters.size());
		for (const auto& param : desc.parameters) {
			D3D12_ROOT_PARAMETER elm;
			elm.ParameterType = TypeConverter::Convert(param.type);
			elm.ShaderVisibility = TypeConverter::Convert(param.visibility);
			switch (param.type)
			{
			case RootParameterType::Range:
			{
				CD3DX12_DESCRIPTOR_RANGE range(
					TypeConverter::Convert(param.range.type),
					param.range.num,
					param.range.baseRegister,
					param.range.registerSpace
				);
				descriptors.push_back(range);

				elm.DescriptorTable.NumDescriptorRanges = 1;
				elm.DescriptorTable.pDescriptorRanges = descriptors.data() + descriptors.size() - 1;
				break;
			}
			case RootParameterType::RootConstants:
				elm.Constants.Num32BitValues = param.constants.value;
				elm.Constants.ShaderRegister = param.constants.registerNo;
				elm.Constants.RegisterSpace = param.constants.registerSpace;
				break;
			default:
				elm.Descriptor.ShaderRegister = param.descriptor.registerNo;
				elm.Descriptor.RegisterSpace = param.descriptor.registerSpace;
				break;
			}
			parameters.push_back(elm);
		}


		// 静的サンプラー
		Array<CD3DX12_STATIC_SAMPLER_DESC> samplerDescs;
		samplerDescs.reserve(desc.samplers.size());
		for (auto& params : desc.samplers) {
			CD3DX12_STATIC_SAMPLER_DESC sampler(
				params.registerNo,
				TypeConverter::Convert(params.sampler.filter, params.sampler.filter, params.sampler.mipFilter, params.sampler.anisotropy != Anisotropy::None),
				TypeConverter::Convert(params.sampler.addressU),
				TypeConverter::Convert(params.sampler.addressV),
				TypeConverter::Convert(params.sampler.addressW),
				params.sampler.mipLodBias,
				TypeConverter::Convert(params.sampler.anisotropy),
				D3D12_COMPARISON_FUNC_LESS_EQUAL,
				D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
				params.sampler.mipLodRange.min,
				params.sampler.mipLodRange.max,
				TypeConverter::Convert(params.visibility),
				params.registerSpace
			);
			samplerDescs.push_back(sampler);
		}


		// ルートシグネチャディスクリプタ
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
			(UINT)parameters.size(), parameters.data(),
			(UINT)samplerDescs.size(), samplerDescs.data(),
			TypeConverter::Convert(desc.flags)
		);



		// バイナリデータの作成
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("D3D12SerializeRootSignature"));
			if (errorBlob) {
				StringBase<char> s;
				s.resize(errorBlob->GetBufferSize());
				std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), s.begin());
			}
			return;
		}

		// ルートシグネチャの作成
		result = rDevice.getNative()->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateRootSignature()"));
			return;
		}

		Utility::setName(m_rootSignature.Get(), getName());

	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	RootSignatureImpl::~RootSignatureImpl() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当なオブジェクトか
	//@―---------------------------------------------------------------------------
	bool RootSignatureImpl::isValid()const {
		return m_rootSignature;
	}

	
	//@―---------------------------------------------------------------------------
	//! @brief      名前を取得
	//@―---------------------------------------------------------------------------
	const String& RootSignatureImpl::getName()const {
		return m_desc.name;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  定義を取得
	//@―---------------------------------------------------------------------------
	const RootSignatureDesc& RootSignatureImpl::getDesc()const noexcept {
		return m_desc;
	}

}// namespace ob
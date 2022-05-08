//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "RootSignatureImpl.h"
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    RootSignatureImpl::RootSignatureImpl(DeviceImpl& rDevice, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		// パラメータ
		vector<D3D12_ROOT_PARAMETER> parameters;
		vector<CD3DX12_DESCRIPTOR_RANGE> descriptors;
		parameters.reserve(desc.parameters.size());
		for (const auto& param : desc.parameters) {
			D3D12_ROOT_PARAMETER elm;
			elm.ParameterType = TypeConverter::convert(param.type);
			elm.ShaderVisibility = TypeConverter::convert(param.visibility);
			switch (param.type)
			{
			case RootParameterType::DescriptorTable:
			{
				size_t start = descriptors.size();
				for (const auto& param2 : param.descriptors) {
					CD3DX12_DESCRIPTOR_RANGE range(
						TypeConverter::convert(param2.type),
						param2.num,
						param2.baseRegister,
						param2.registerSpace
					);
					descriptors.push_back(range);
				}
				elm.DescriptorTable.NumDescriptorRanges=(UINT)param.descriptors.size();
				elm.DescriptorTable.pDescriptorRanges= descriptors.data()+ start;
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
		// TODO テクスチャごとのサンプラーを定義するか検討
		vector<CD3DX12_STATIC_SAMPLER_DESC> samplerDescs;
		samplerDescs.reserve(desc.samplers.size());
		for (auto& params: desc.samplers) {
			CD3DX12_STATIC_SAMPLER_DESC sampler(
				params.registerNo,
				TypeConverter::convert(params.sampler.filter, params.sampler.filter, params.sampler.mipFilter, params.sampler.anisotropy!=Anisotropy::None),
				TypeConverter::convert(params.sampler.addressU),
				TypeConverter::convert(params.sampler.addressV),
				TypeConverter::convert(params.sampler.addressW),
				params.sampler.mipLodBias,
				TypeConverter::convert(params.sampler.anisotropy),
				D3D12_COMPARISON_FUNC_LESS_EQUAL,
				D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
				params.sampler.mipLodRange.min,
				params.sampler.mipLodRange.max,
				TypeConverter::convert(params.visibility),
				params.registerSpace
				);
			samplerDescs.push_back(sampler);
		}


		// ルートシグネチャディスクリプタ
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
			(UINT)parameters.size(), parameters.data(),
			(UINT)samplerDescs.size(), samplerDescs.data(),
			TypeConverter::convert(desc.flags)
		);



		// バイナリデータの作成
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("D3D12SerializeRootSignature"));
			if (errorBlob) {
				StringBase<char> s;
				s.resize(errorBlob->GetBufferSize());
				std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), s.begin());
			}
			return;
		}

		// ルートシグネチャの作成
		result = rDevice.getNativeDevice()->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("ID3D12Device::CreateRootSignature()"));
			return;
		}

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
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const RootSignatureDesc& RootSignatureImpl::getDesc()const noexcept {
        return m_desc;
    }

}// namespace ob
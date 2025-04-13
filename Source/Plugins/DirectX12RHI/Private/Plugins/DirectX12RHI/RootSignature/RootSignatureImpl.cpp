//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "RootSignatureImpl.h"
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>

namespace ob::rhi::dx12 {

	static D3D12_DESCRIPTOR_RANGE_TYPE Convert(BindingType value) {
		switch (value)
		{
		case BindingType::Texture:
		case BindingType::Buffer:
		case BindingType::StructuredBuffer:
		case BindingType::ByteAddressBuffer:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

		case BindingType::RWTexture:
		case BindingType::RWBuffer:
		case BindingType::RWStructuredBuffer:
		case BindingType::RWByteAddressBuffer:
			return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

		case BindingType::ConstantBuffer:
			return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

		case BindingType::Sampler:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		}
		OB_ABORT("不正なRootParameterTypeです。");
		return {};
	}

	//! @brief  コンストラクタ
	RootSignatureImpl::RootSignatureImpl(DirectX12RHI& rDevice, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		// パラメータ
		Vector<D3D12_ROOT_PARAMETER> parameters;
		Vector<CD3DX12_DESCRIPTOR_RANGE> ranges;
		parameters.reserve(100);
		ranges.reserve(100);

		// インデックスを正規化
		s32 index = 0;
		for (auto& slot : m_desc.slots) {
			for (auto [i, item] : Indexed(slot.items)) {
				// 先頭がオフセット指定ならば0に置き換え
				if (i == 0 && item.index < 0) {
					item.index = 0;
				}
				// オフセット指定なら正規化
				if (item.index < 0) {
					item.index = index - item.index;
				}
				index = item.index;
			}
		}

		// テーブル
		for (auto& slot : m_desc.slots) {

			s32 rangeStart = (s32)ranges.size();

			auto& parameter = parameters.emplace_back();
			parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			parameter.DescriptorTable.pDescriptorRanges = ranges.data() + rangeStart;


			s32 numSampler = 0;
			s32 numResource = 0;


			// 連続する領域をCD3DX12_DESCRIPTOR_RANGEにまとめる
			// NOTE 複雑なことをしなくても常にNumDescriptorsを1にしておけばよいのでは？
			BindingItem last(BindingType::Texture, -1, -1);

			for (auto [i, item] : Indexed(slot.items)) {

				if (Convert(last.type) != Convert(item.type) || last.index + 1 != item.index || last.space != item.space) {
					auto& range = ranges.emplace_back();
					range.RangeType = Convert(item.type);
					range.NumDescriptors = 0;
					range.BaseShaderRegister = item.index;
					range.RegisterSpace = item.space;
					range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
				}

				auto& range = ranges.back();
				range.NumDescriptors++;

				last = item;

				if (item.type == BindingType::Sampler) {
					numSampler++;
				}
				else {
					numResource++;
				}

			}

			if (0 < numSampler && 0 < numResource) {
				LOG_ERROR("BindingSlot内にサンプラーとリソースが混在しています。 [name={}]", m_desc.name);
				return;
			}

			parameter.DescriptorTable.NumDescriptorRanges = ranges.size() - rangeStart;


			// 要素数が1かつRootDescriptorが視聴できるのであれば切り替え
			if (parameter.DescriptorTable.NumDescriptorRanges = 1) {
				switch (parameter.DescriptorTable.pDescriptorRanges[0].RangeType) {
					// テクスチャなどはRootDescriptorに指定できないのでとりあえず無効化。
					// TODO StructuredBufferは使えるので対応する
					//case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
					//	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
					//	break;
					//case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
					//	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
					//	break;
					//case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
					//	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
					//	parameter.Descriptor.RegisterSpace = parameter.DescriptorTable.pDescriptorRanges[0].RegisterSpace;
					//	parameter.Descriptor.ShaderRegister = parameter.DescriptorTable.pDescriptorRanges[0].BaseShaderRegister;
					//	break;
					//}
				}

			}
		}

		// 静的サンプラー
		Vector<CD3DX12_STATIC_SAMPLER_DESC> samplerDescs;
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

		// ルート定数
		if (0 < desc.constants.size) {
			if (desc.constants.size % sizeof(u32) != 0) {
				LOG_WARNING("RootConstantのサイズが4の倍数ではありません。[size={}]", desc.constants.size);
				return;
			}

			D3D12_ROOT_PARAMETER elm;
			elm.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			elm.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			elm.Constants.Num32BitValues = desc.constants.size;
			elm.Constants.ShaderRegister = desc.constants.registerNo;
			elm.Constants.RegisterSpace = desc.constants.registerSpace;

			parameters.push_back(elm);
		}


		// ルートシグネチャディスクリプタ
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
			(UINT)parameters.size(), parameters.data(),
			(UINT)samplerDescs.size(), samplerDescs.data(),
			TypeConverter::Convert(desc.flags)
		);

		rootSignatureDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// Bindless設定(ShaderModel6.6～)
		if (desc.flags.has(RootSignatureFlag::EnableBindless)) {
			rootSignatureDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED;
			rootSignatureDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;
		}


		// バイナリデータの作成
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		if (FAILED(result)) {
			String s;
			if (errorBlob) {
				s.resize(errorBlob->GetBufferSize());
				std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), s.begin());
			}
			Utility::OutputFatalLog(result, "D3D12SerializeRootSignature");
			return;
		}

		// ルートシグネチャの作成
		result = rDevice.getNative()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "ID3D12Device::CreateRootSignature()");
			return;
		}

		Utility::SetName(m_rootSignature.Get(), getName());

	}

	//! @brief  デストラクタ
	RootSignatureImpl::~RootSignatureImpl() {

	}


	//! @brief  妥当なオブジェクトか
	bool RootSignatureImpl::isValid()const {
		return m_rootSignature;
	}


	//! @brief      名前を取得
	const String& RootSignatureImpl::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const RootSignatureDesc& RootSignatureImpl::getDesc()const noexcept {
		return m_desc;
	}

}
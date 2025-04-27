//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "DirectX12RootSignature.h"
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorLayout.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	DirectX12RootSignature::DirectX12RootSignature(DirectX12RHI& rDevice, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		// パラメータ
		Vector<D3D12_ROOT_PARAMETER> parameters;
		Vector<CD3DX12_DESCRIPTOR_RANGE> ranges;
		parameters.reserve(100);
		ranges.reserve(100);

		s32 slot = 0;

		// ルート定数
		// Vulkan対応のためRootConstantsは0版固定で作成する
		if (0 < desc.constants.size) {
			if (desc.constants.size % sizeof(u32) != 0) {
				LOG_WARNING("RootConstantのサイズが4の倍数ではありません。[size={}]", desc.constants.size);
				return;
			}

			D3D12_ROOT_PARAMETER elm;
			elm.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			elm.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			elm.Constants.Num32BitValues = desc.constants.size / 4;
			elm.Constants.ShaderRegister = desc.constants.registerNo;
			elm.Constants.RegisterSpace = desc.constants.registerSpace;

			parameters.push_back(elm);
		}

		// テーブル
		for (auto& layout : m_desc.layouts) {

			// DirectX12ではSamplerとCBV_SRV_UAVは別のヒープに分ける必要があるのでlayoutに対して複数のD3D12_ROOT_PARAMETERを設定する
			DescriptorHeapType heapTypes[] = { DescriptorHeapType::Sampler, DescriptorHeapType::CBV_SRV_UAV };
			const auto heapTypeOf = [](BindingType type) { return type == BindingType::Sampler ? DescriptorHeapType::Sampler : DescriptorHeapType::CBV_SRV_UAV; };

			MapInfo& mapInfo = m_mapInfos.emplace_back();
			auto& items = layout->getDesc().items;

			for (auto heapType : heapTypes) {

				s32 rangeStart = (s32)ranges.size();

				auto& parameter = parameters.emplace_back();
				parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				parameter.DescriptorTable.pDescriptorRanges = ranges.data() + rangeStart;

				// 連続する領域をCD3DX12_DESCRIPTOR_RANGEにまとめる
				BindingItem last(BindingType::Texture, -1, -1);

				for (s32 i = 0; i < items.size(); i++) {

					auto lastType = TypeConverter::Convert(last.type);
					auto itemType = TypeConverter::Convert(items[i].type);

					if (lastType != itemType || last.index + 1 != items[i].index || last.space != items[i].space) {

						// Heapが異なる要素はスキップ
						while (i < items.size() && heapTypeOf(items[i].type) != heapType) ++i;
						if (i == items.size()) break;

						auto& range = ranges.emplace_back();
						range.RangeType = TypeConverter::Convert(items[i].type);
						range.NumDescriptors = 0;
						range.BaseShaderRegister = items[i].index;
						range.RegisterSpace = items[i].space;
						range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					}

					auto& range = ranges.back();
					range.NumDescriptors++;

					last = items[i];

				}

				// 要素がない場合はスキップ
				if (ranges.size() == rangeStart) {
					parameters.pop_back();
					continue;
				}

				// RHI層のSlotとDirectX12層のSlotをマッピング
				if (heapType == DescriptorHeapType::Sampler) mapInfo.samplerSlot = parameters.size() - 1;
				if (heapType == DescriptorHeapType::CBV_SRV_UAV) mapInfo.othersSlot = parameters.size() - 1;

				// このループで追加されたRange数 == NumDescriptorRanges
				parameter.DescriptorTable.NumDescriptorRanges = ranges.size() - rangeStart;


				// TODO 要素数が1かつRootDescriptorが視聴できるのであれば切り替え
				if (parameter.DescriptorTable.NumDescriptorRanges == 1) {
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

		manage();
	}

	//! @brief  デストラクタ
	DirectX12RootSignature::~DirectX12RootSignature() {

	}


	//! @brief  妥当なオブジェクトか
	bool DirectX12RootSignature::isValid()const {
		return m_rootSignature;
	}


	//! @brief      名前を取得
	const String& DirectX12RootSignature::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const RootSignatureDesc& DirectX12RootSignature::getDesc()const noexcept {
		return m_desc;
	}

}
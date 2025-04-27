//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "DirectX12DescriptorTable.h"
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorLayout.h>
#include <Plugins/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/Sampler/DirectX12Sampler.h>
#include <Plugins/DirectX12RHI/RootSignature/DirectX12RootSignature.h>

namespace ob::rhi
{

	//! @brief              コンストラクタ
	//!
	//! @param type         デスクリプタに設定するリソースの種類
	//! @param elementNum   要素数
	DirectX12DescriptorTable::DirectX12DescriptorTable(DirectX12RHI& rhi, const DescriptorTableDesc& desc, DescriptorHeap& heap0, DescriptorHeap& heap1)
		: m_rhi(rhi)
		, m_desc(desc)
	{
		m_layout = m_desc.layout.cast<DirectX12DescriptorLayout>();
		if (m_layout == nullptr) return;

		auto heapInfo = m_layout->getHeapInfo();
		heap0.allocateHandle(m_samplerHandle, heapInfo.samplerNum);
		heap1.allocateHandle(m_othersHandle, heapInfo.othersNum);

		m_elemetns.resize(m_layout->getDesc().items.size());

		manage();
	}


	//! @brief  妥当な状態か
	bool DirectX12DescriptorTable::isValid()const {
		return !m_samplerHandle.empty() || !m_othersHandle.empty();
	}


	//! @brief      名前を取得
	const String& DirectX12DescriptorTable::getName()const {
		return m_desc.name;
	}


	//! @brief  バッファリソースを設定
	bool DirectX12DescriptorTable::setResource(s32 index,const Ref<Buffer>& resource) {

		D3D12_DESCRIPTOR_RANGE_TYPE type;

		if (!resource) {
			LOG_ERROR("不正な引数。DescriptorTableに空のバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!tryGetRangeType(index, resource, type)) {
			LOG_ERROR("不正な呼び出し。異なるタイプのDescriptorTableにバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns.at(index) = resource;

		if (auto p = resource.cast<DirectX12Buffer>()) {
			auto mapInfo = m_layout->getMapInfo(index);
			auto handle = m_othersHandle.getCpuHandle(mapInfo.index);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_CBV)p->createCBV(handle);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_SRV)p->createSRV(handle);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)p->createUAV(handle);
		}
		return true;
	}


	//! @brief  テクスチャリソースを設定
	bool DirectX12DescriptorTable::setResource(s32 index, const Ref<Texture>& resource) {

		D3D12_DESCRIPTOR_RANGE_TYPE type;

		if (!resource) {
			LOG_ERROR("不正な引数。DescriptorTableに空のテクスチャを指定しました。[index={}]", index);
			return false;
		}
		if (!tryGetRangeType(index, resource, type)) {
			LOG_ERROR("不正な呼び出し。異なるタイプのDescriptorTableにバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns.at(index) = resource;

		if (auto p = resource.cast<DirectX12Texture>()) {
			auto mapInfo = m_layout->getMapInfo(index);
			auto handle = m_othersHandle.getCpuHandle(mapInfo.index);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_SRV)p->createSRV(handle);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)p->createUAV(handle,0);
		}
		return true;
	}

	//! @brief  サンプラーリソースを設定
	bool DirectX12DescriptorTable::setResource(s32 index, const Ref<Sampler>& resource) {

		D3D12_DESCRIPTOR_RANGE_TYPE type;

		if (!resource) {
			LOG_ERROR("不正な引数。DescriptorTableに空のサンプラーを指定しました。[index={}]", index);
			return false;
		}
		if (!tryGetRangeType(index, resource, type)) {
			LOG_ERROR("不正な呼び出し。異なるタイプのDescriptorTableにバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns.at(index) = resource;

		if (auto p = resource.cast<DirectX12Sampler>()) {
			auto mapInfo = m_layout->getMapInfo(index);
			auto source = p->getCopyableHandle();
			auto dest = m_samplerHandle.getCpuHandle(mapInfo.index);
			
			m_rhi.getNative()->CopyDescriptorsSimple(1, dest, source, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		}
		return true;
	}

	//! @brief  バインドレスハンドルに使用するインデックスを取得
	BindlessHandle DirectX12DescriptorTable::getBindlessHandle(s32 index)const {

		auto& items = m_layout->getDesc().items;
		auto mapInfo = m_layout->getMapInfo(index);

		BindlessHandle handle;
		handle.type = items.at(index).type;
		if (mapInfo.type == DescriptorHeapType::Sampler) handle.index = m_samplerHandle.getBindlessIndex(mapInfo.index);
		if (mapInfo.type == DescriptorHeapType::CBV_SRV_UAV) handle.index = m_othersHandle.getBindlessIndex(mapInfo.index);

		return handle;
	}


	//! @brief CommandListに記録 
	void DirectX12DescriptorTable::record(ID3D12GraphicsCommandList& cmdList,DirectX12RootSignature& signature,s32 slot) const {

		bool isRootDescriptor = false;
		if (isRootDescriptor) {
			// D3D12_GPU_VIRTUAL_ADDRESS address;
			// cmdList.SetGraphicsRootConstantBufferView(m_slot, address);
			// cmdList.SetGraphicsRootShaderResourceView(m_slot, address);
			// cmdList.SetGraphicsRootUnorderedAccessView(m_slot, address);
		} else {

			auto mapInfo = signature.getMapInfo(slot);

			if (!m_samplerHandle.empty()) {
				OB_ASSERT_EXPR(0<=mapInfo.samplerSlot);
				cmdList.SetGraphicsRootDescriptorTable(mapInfo.samplerSlot, m_samplerHandle.getGpuHandle());
			}
			if (!m_othersHandle.empty()) {
				OB_ASSERT_EXPR(0 <= mapInfo.othersSlot);
				cmdList.SetGraphicsRootDescriptorTable(mapInfo.othersSlot, m_othersHandle.getGpuHandle());
			}

		}
	}




	bool DirectX12DescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, D3D12_DESCRIPTOR_RANGE_TYPE& type) const {

		auto& items = m_layout->getDesc().items;
		if (!buffer) return false;
		if(!is_in_range(index, items)) return false;

		auto& desc = buffer->getDesc();
		bool hasSRV = desc.flags.has(BufferFlag::ShaderResource);
		bool hasUAV = desc.flags.has(BufferFlag::UnorderedAccess);

		switch (items[index].type) {
		case BindingType::Buffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			return hasSRV;

		case BindingType::RWBuffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			return hasUAV;

		case BindingType::StructuredBuffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			return hasSRV && 0 < desc.stride;

		case BindingType::RWStructuredBuffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			return hasUAV && 0 < desc.stride;

		case BindingType::ByteAddressBuffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			return hasSRV;

		case BindingType::RWByteAddressBuffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			return hasUAV;

		case BindingType::ConstantBuffer:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			return desc.state == BufferState::Constant;
		}
		return false;
	}
	bool DirectX12DescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, D3D12_DESCRIPTOR_RANGE_TYPE& type) const {

		auto& items = m_layout->getDesc().items;
		if (!texture) return false;
		if (!is_in_range(index, items)) return false;

		auto& desc = texture->desc();
		bool hasSRV = desc.flags.has(TextureFlag::ShaderResource);
		bool hasUAV = desc.flags.has(TextureFlag::UnorderedAccess);

		switch (items[index].type) {
		case BindingType::Texture:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			return hasSRV;

		case BindingType::RWTexture:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			return hasUAV;
		}
		return false;
	}
	bool DirectX12DescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, D3D12_DESCRIPTOR_RANGE_TYPE& type) const {

		auto& items = m_layout->getDesc().items;
		if (!sampler) return false;
		if (!is_in_range(index, items)) return false;

		switch (items[index].type) {
		case BindingType::Sampler:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
			return true;
		}
		return false;
	}

}
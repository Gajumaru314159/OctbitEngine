//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "DescriptorTableImpl.h"
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>
#include <Plugins/DirectX12RHI/Sampler/SamplerImpl.h>
#include <Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>

namespace ob::rhi::dx12
{

	//! @brief              コンストラクタ
	//!
	//! @param type         デスクリプタに設定するリソースの種類
	//! @param elementNum   要素数
	DescriptorTableImpl::DescriptorTableImpl(DirectX12RHI& rhi, DescriptorHeap& heap, const Ref<RootSignature>& signature, s32 slot)
		: m_rhi(rhi)
		, m_signature(signature.cast<RootSignatureImpl>())
		, m_slot(slot)
	{
		if (m_signature == nullptr) return;
		s32 itemCount = m_signature->getItemCount(slot);

		heap.allocateHandle(m_handle, itemCount);

		m_elemetns.resize(itemCount);
	}

	DescriptorTableImpl::DescriptorTableImpl(DirectX12RHI& rhi, DescriptorHeap& heap, const BindingSlot& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{
		heap.allocateHandle(m_handle, desc.items.size());

		m_elemetns.resize(desc.items.size());
	}


	//! @brief  妥当な状態か
	bool DescriptorTableImpl::isValid()const {
		return !m_handle.empty();
	}


	//! @brief      名前を取得
	const String& DescriptorTableImpl::getName()const {
		return m_name;
	}


	//! @brief  バッファリソースを設定
	bool DescriptorTableImpl::setResource(s32 index,const Ref<Buffer>& resource) {

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

		if (auto p = resource.cast<BufferImpl>()) {
			auto handle = m_handle.getCpuHandle(index);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_CBV)p->createCBV(handle);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_SRV)p->createSRV(handle);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)p->createUAV(handle);
		}
		return true;
	}


	//! @brief  テクスチャリソースを設定
	bool DescriptorTableImpl::setResource(s32 index, const Ref<Texture>& resource) {

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

		if (auto p = resource.cast<TextureImpl>()) {
			auto handle = m_handle.getCpuHandle(index);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_SRV)p->createSRV(handle);
			if (type == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)p->createUAV(handle,0);
		}
		return true;
	}

	//! @brief  サンプラーリソースを設定
	bool DescriptorTableImpl::setResource(s32 index, const Ref<Sampler>& resource) {

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

		if (auto p = resource.cast<SamplerImpl>()) {
			auto source = p->getCopyableHandle();
			auto dest = m_handle.getCpuHandle(index);
			
			m_rhi.getNative()->CopyDescriptorsSimple(1, dest, source, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		}
		return true;
	}

	//! @brief CommandListに記録 
	void DescriptorTableImpl::record(ID3D12GraphicsCommandList& cmdList,s32 slot) const {

		if (slot < 0) slot = m_slot;
		OB_ASSERT(0<=slot,"スロット指定が必要です");

		bool isRootDescriptor = false;
		if (isRootDescriptor) {
			// D3D12_GPU_VIRTUAL_ADDRESS address;
			// cmdList.SetGraphicsRootConstantBufferView(m_slot, address);
			// cmdList.SetGraphicsRootShaderResourceView(m_slot, address);
			// cmdList.SetGraphicsRootUnorderedAccessView(m_slot, address);
		} else {
			cmdList.SetGraphicsRootDescriptorTable(slot, getGpuHandle());
		}
	}




	bool DescriptorTableImpl::tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, D3D12_DESCRIPTOR_RANGE_TYPE& type) const {

		auto& slot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		if (!buffer) return false;
		if (!is_in_range(index, slot.items)) return false;

		auto& desc = buffer->getDesc();
		bool hasSRV = desc.flags.has(BufferFlag::ShaderResource);
		bool hasUAV = desc.flags.has(BufferFlag::UnorderedAccess);

		switch (slot.items[index].type) {
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
			return desc.state == BufferState::ConstantBuffer;
		}
		return false;
	}
	bool DescriptorTableImpl::tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, D3D12_DESCRIPTOR_RANGE_TYPE& type) const {

		auto& slot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		if (!texture) return false;
		if (!is_in_range(index, slot.items)) return false;

		auto& desc = texture->desc();
		bool hasSRV = desc.flags.has(TextureFlag::ShaderResource);
		bool hasUAV = desc.flags.has(TextureFlag::UnorderedAccess);

		switch (slot.items[index].type) {
		case BindingType::Texture:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			return hasSRV;

		case BindingType::RWTexture:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			return hasUAV;
		}
		return false;
	}
	bool DescriptorTableImpl::tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, D3D12_DESCRIPTOR_RANGE_TYPE& type) const {

		auto& slot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		if (!sampler) return false;
		if (!is_in_range(index, slot.items)) return false;

		switch (slot.items[index].type) {
		case BindingType::Sampler:
			type = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
			return true;
		}
		return false;
	}

}
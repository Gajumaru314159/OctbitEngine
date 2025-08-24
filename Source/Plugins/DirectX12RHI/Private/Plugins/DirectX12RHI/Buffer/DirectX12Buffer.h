//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>
#include "SmallBufferAllocator.h"

namespace ob::rhi {

	//! @brief      バッファ
	class DirectX12Buffer:public rhi::Buffer {
	public:

		//! @brief  コンストラクタ
		DirectX12Buffer(class DirectX12Device& device,const BufferDesc& desc);


		//! @brief  コンストラクタ
		DirectX12Buffer(class DirectX12Device& device, const BufferDesc& desc, const Blob& blob);


		//! @brief  コンストラクタ
		DirectX12Buffer(class DirectX12Device& device, const BufferViewDesc& desc);


		//! @brief  デストラクタ
		~DirectX12Buffer() override;


		//! @brief  妥当な状態か
		bool isValid()const;


		//! @brief      名前を取得
		const String& getName()const override;


		//! @brief  定義を取得
		const BufferDesc& getDesc()const override;


		//! @brief      BindlessHandleを取得
		BindlessHandle getHandle()const override;


		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		void update(size_t size, const void* pData, size_t offset) override;


		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		void update(const CopyFunc& func) override;


		//! @brief      バッファを更新(直接更新)
		void updateDirect(size_t size, const void* pData, size_t offset) override;


		//! @brief      バッファを更新(直接更新)
		void updateDirect(const CopyFunc& func) override;


	public:

		//! @brief      ネイティブオブジェクトを取得
		ID3D12Resource* getNative()const {return m_resource.Get();}

		//! @brief      ビューのオフセットを取得
		size_t getViewOffset()const {return m_allocation.offset;}

		//! @brief      CBVを生成
		void createCBV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const;

		//! @brief      SRVを生成
		void createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const;

		//! @brief      UAVを生成
		void createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const;


	private:

		DirectX12Device&			m_device;
		BufferDesc					m_desc;
		BufferViewDesc				m_viewDesc;
		DescriptorHandle			m_handle;
		ComPtr<ID3D12Resource>		m_resource;
		
		// サブアロケーション管理用
		BufferAllocation			m_allocation;		//!< アロケーション情報（解放用）

	};


	//! @cond


	//! @brief      BindlessHandleを取得
	inline BindlessHandle DirectX12Buffer::getHandle()const {
		if (m_handle.empty()) return {};
		BindlessHandle handle;
		handle.type = BindingType::ByteAddressBuffer;
		handle.index = m_handle.getBindlessIndex();
		return handle;
	}


	//! @endcond

}
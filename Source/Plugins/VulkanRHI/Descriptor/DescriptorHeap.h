//***********************************************************
//! @file
//! @brief		デスクリプタ・ヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/DescriptorDesc.h>
#include <Framework/Core/Allocator/Utility/TLSFMapper.h>
#include <Plugins/VulkanRHI/Descriptor/DescriptorHeapType.h>

namespace ob::rhi::vulkan {

	//! @brief      デスクリプタのアロケータ
	//! 
	//! @details    Two-Level Segregate Fit を利用してデスクリプタを割り当てます。
	class DescriptorHeap :private Noncopyable {
		friend class DescriptorHandle;
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//! @brief          コンストラクタ
		//! 
		//! @param device   デバイス
		//! @param type     ヒープ・タイプ
		//! @param capacity 容量
		DescriptorHeap(class VulkanRHI& rhi, vk::DescriptorType type, s32 capacity);


		//! @brief          デストラクタ
		~DescriptorHeap();


		//! @brief          ハンドルをアロケート
		//! 
		//! @param handle   アロケート先ハンドル
		//! @param size  割り当て個数
		void allocateHandle(class DescriptorHandle& handle, s32 size);


		//! @brief          ハンドルを解放
		void releaseHandle(class DescriptorHandle& handle);
		

		//! @brief  タイプを取得
		DescriptorHeapType getHeapType()const;


	private:

		Mutex							m_mutex;		//!< ミューテックス
		vk::DescriptorType				m_type;
		vk::raii::DescriptorPool		m_pool = nullptr;	//!< ヒープ	
		u32								m_descriptorSize;

	};

}
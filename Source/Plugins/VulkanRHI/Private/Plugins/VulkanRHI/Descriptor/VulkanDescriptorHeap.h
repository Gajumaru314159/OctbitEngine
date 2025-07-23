//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Allocator/Utility/TLSFMapper.h>

namespace ob::rhi {

	//! @brief      デスクリプタのアロケータ
	//! 
	//! @details    Two-Level Segregate Fit を利用してデスクリプタを割り当てます。
	class VulkanDescriptorHeap :private Noncopyable {
		friend class DescriptorHandle;
	public:

		//! @brief          コンストラクタ
		//! 
		//! @param device   デバイス
		//! @param type     ヒープ・タイプ
		//! @param resourceCapacity 容量
		//! @param samplerCapacity  サンプラー容量
		VulkanDescriptorHeap(class VulkanRHI& device, s32 resourceCapacity, s32 samplerCapacity);


		//! @brief          デストラクタ
		~VulkanDescriptorHeap();


		//! @brief          ハンドルをアロケート
		//! 
		//! @param handle   アロケート先ハンドル
		//! @param desc  割り当て先の記述セット
		void allocateHandle(class VulkanDescriptorHandle& handle, vk::WriteDescriptorSet& desc);


		//! @brief          ハンドルを解放
		void releaseHandle(class VulkanDescriptorHandle& handle);

		//! @birief			更新
		void update();


		//! @brief          デスクリプタセットをコマンドバッファに記録
		void recordDescriptorHeap(vk::raii::CommandBuffer& commandBuffer,vk::PipelineLayout pipeline,s32 slot);


		//! @brief			レイアウトを取得
		//! @details		RootSignature内部でPipelineLayoutを生成するときに使用されます。
		vk::DescriptorSetLayout getLayout() const { return *m_layout; }

	private:

		void initializeStaged();
		void initializeStaging();

	private:

		SpinLock						m_mutex;
		TLSFMapper						m_samplerMapper;
		TLSFMapper						m_resourceMapper;

		using SubRequest = Variant<vk::DescriptorImageInfo, vk::DescriptorBufferInfo>;

		SpinLock						m_requestsLock;
		Vector<vk::WriteDescriptorSet>	m_requests;
		Vector<SubRequest>				m_subrequests;

		vk::raii::DescriptorPool		m_pool = nullptr;
		vk::raii::DescriptorSet			m_set = nullptr;
		vk::raii::DescriptorSetLayout	m_layout = nullptr;

	};

}
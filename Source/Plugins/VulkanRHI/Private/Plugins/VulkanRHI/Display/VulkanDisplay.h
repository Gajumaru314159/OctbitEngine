//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Display.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/Core/Utility/Swapper.h>
#include <Plugins/VulkanRHI/Command/VulkanResourceStateCache.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::vulkan {

	class VulkanRHI;

	class VulkanDisplay :public Display{
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		VulkanDisplay(VulkanRHI& rhi, const DisplayDesc& desc);
		~VulkanDisplay();

		const String& getName()const override {
			return m_desc.name;
		}

		//! @brief      定義を取得
		const DisplayDesc& getDesc()const noexcept;

		//! @brief      更新
		void update() override;
		void update(vk::Queue queue);

		//! @brief      イベントリスナ追加
		void addEventListener(DisplayEventHandle& handle, DisplayEventDelegate func);

	public:

		//! @brief      テクスチャをディスプレイにコピー
		void recordApplyDisplay(Ref<CommandList>& cmdList, const Ref<RenderTexture>& texture);

	private:

		void createResources(VulkanRHI& rhi);

	private:

		VulkanRHI&					m_rhi;
		DisplayDesc					m_desc;

		vk::raii::SurfaceKHR		m_surface = nullptr;
		vk::raii::SwapchainKHR		m_swapchain = nullptr;
		Swapper<vk::ImageView>		m_imageViews;
		Vector<vk::raii::ImageView>m_imageViews2;
		Vector<vk::Image>			m_images;
		vk::raii::Fence				m_fence = nullptr;	

		// 描画リソース    
		Ref<RootSignature>          m_signature;
		Ref<PipelineState>          m_pipeline;
		Ref<Buffer>                 m_verices;
		Ref<Texture>                m_bindedTexture;
		Ref<Sampler>                m_bindedSampler;
		Ref<DescriptorTable>        m_bindedTextureTable;
		Ref<DescriptorTable>        m_bindedSamplerTable;

		VulkanResourceStateCache	m_cache;
	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

}
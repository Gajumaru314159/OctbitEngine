//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/Core/Utility/Swapper.h>
#include <Plugins/VulkanRHI/Command/VulkanResourceStateCache.h>

namespace ob::rhi::vulkan {

	class VulkanSwapChain :public SwapChain{
	public:

		VulkanSwapChain(VulkanRHI& rhi, const SwapChainDesc& desc);
		~VulkanSwapChain();

		const String& getName()const override {
			return m_desc.name;
		}

		//! @brief      定義を取得
		const SwapChainDesc& getDesc()const noexcept;

		//! @brief      更新
		void update() override;
		void update(vk::Queue queue);

		//! @brief      イベントリスナ追加
		void addEventListener(SwapChainEventHandle& handle, SwapChainEventDelegate func);

	public:

		//! @brief      テクスチャをスワップチェーンにコピー
		void recordApplySwapChain(Ref<CommandList>& cmdList, const Ref<RenderTexture>& texture);

	private:

		void createResources(VulkanRHI& rhi);

		void onWindowChanged(const platform::WindowEventArgs& args);

	private:

		VulkanRHI&					m_rhi;
		SwapChainDesc					m_desc;

		vk::raii::SurfaceKHR		m_surface = nullptr;
		vk::raii::SwapchainKHR		m_swapchain = nullptr;
		Swapper<vk::ImageView>		m_imageViews;
		Vector<vk::raii::ImageView>	m_imageViews2;
		Vector<vk::Image>			m_images;
		vk::raii::Fence				m_fence = nullptr;	

		// 描画リソース    
		Ref<DescriptorLayout>       m_layout;
		Ref<RootSignature>          m_signature;
		Ref<PipelineState>          m_pipeline;
		Ref<Buffer>                 m_verices;
		Ref<Texture>                m_bindedTexture;
		Ref<Sampler>                m_bindedSampler;
		Ref<DescriptorTable>        m_table;

		bool						m_visible = false;	

		VulkanResourceStateCache	m_cache;

		SwapChainEventNotifier        m_notifier;
	};
}
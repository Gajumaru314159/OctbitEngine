//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/Core/Utility/Swapper.h>
#include <Framework/Platform/Type/SystemEventType.h>
#include <Framework/VulkanRHI/Command/VulkanResourceStateCache.h>

namespace ob::rhi {

	class VulkanSwapChain :public SwapChain{
	public:

		VulkanSwapChain(VulkanDevice& device, const SwapChainDesc& desc);
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

		void createResources();

		void createBuffer();

		void clearBuffer();

		void onWindowChanged(const platform::WindowEventArgs& args);

	private:

		VulkanDevice&					m_device;
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

		bool						m_visible = true;	

		VulkanResourceStateCache	m_cache;

		SwapChainEventNotifier        m_notifier;

		platform::WindowEventHandle m_hEvent;

		bool m_firstTimeTest = true;
		bool m_closed = false;
	};
}
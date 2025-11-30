//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/SwapChain/VulkanSwapChain.h>
#include <Plugins/VulkanRHI/VulkanDevice.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Command/VulkanCommandList.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/DescriptorLayout.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Sampler.h>
#include <magic_enum.hpp>

namespace ob::rhi {

	//! @brief  コンストラクタ
	VulkanSwapChain::VulkanSwapChain(VulkanDevice& device, const SwapChainDesc& desc)
		: m_device(device)
	{
		m_desc = desc;
		
		// 未指定の場合はwindowから取得
		if (desc.size.width <= 1 || desc.size.height <= 1) {
			m_desc.size = { (s32)desc.window.getSize().x,(s32)desc.window.getSize().y };
		}

		createResources();
		createBuffer();

		m_desc.window.addEventListener(m_hEvent, { *this,&VulkanSwapChain::onWindowChanged });

		manage();
	}


	//! @brief  デストラクタ
	VulkanSwapChain::~VulkanSwapChain() {
	}


	//! @brief  定義を取得
	const SwapChainDesc& VulkanSwapChain::getDesc()const noexcept {
		return m_desc;
	}

	
	//! @brief 更新
	void VulkanSwapChain::update() {
		update(m_device.getQueue());
	}


	//! @brief 更新 
	void VulkanSwapChain::update(vk::Queue queue) {

		// TODO SwapChainのupdateが初回の描画より前だった場合SwapChainがPreset状態ではないのでスキップ
		if (m_firstTimeTest) {
			m_firstTimeTest = false;
			return;
		}
		if (m_closed) return;
		if (!m_visible) return;

		if (!m_desc.window.isValid())return;

		auto& device = m_device.getDevice();

		device.resetFences(*m_fence);

		auto [result,index] = m_swapchain.acquireNextImage(1'000'000'000, {}, *m_fence);
		
		if (result != vk::Result::eSuccess) {
			LOG_ERROR("次フレームの取得に失敗");
			return;
		}

		if (device.waitForFences(*m_fence, VK_TRUE, 1'000'000'000) != vk::Result::eSuccess) {
			LOG_ERROR("フレーム待機に失敗");
			return;
		}



		vk::PresentInfoKHR info;
		info.setSwapchains(*m_swapchain);
		info.setImageIndices(index);
		
		if (queue.presentKHR(info) != vk::Result::eSuccess) {
			LOG_ERROR("Presentに失敗しました。");
		}
		
		queue.waitIdle();

		m_imageViews.next();
	}


	//! @brief      イベントリスナ追加
	void VulkanSwapChain::addEventListener(SwapChainEventHandle& handle, SwapChainEventDelegate func) {
		m_notifier.add(handle, func);
	}


	//! @brief      テクスチャをスワップチェーンにコピー
	void VulkanSwapChain::recordApplySwapChain(Ref<CommandList>& cmdList, const Ref<RenderTexture>& texture) {

		// テクスチャが違う場合再バインド
		if (m_bindedTexture != texture) {
			m_bindedTexture = texture.get();
			m_table->setResource(0, m_bindedTexture);
		}

		if(auto impl = cmdList.cast<VulkanCommandList>()) {

			vk::CommandBuffer commandBuffer = impl->getNative();

			m_cache.clear();
			m_cache.addTexture(m_images[m_imageViews.index()], vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal,vk::ImageAspectFlagBits::eColor);
			m_cache.recordCommand(commandBuffer);

			vk::RenderingAttachmentInfo attachment;
			attachment.imageView = m_imageViews.current();
			attachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
			attachment.loadOp = vk::AttachmentLoadOp::eClear;
			attachment.storeOp = vk::AttachmentStoreOp::eStore;

			vk::RenderingInfo renderingInfo;
			renderingInfo.flags = vk::RenderingFlagBits{};
			renderingInfo.renderArea.offset.x = 0;
			renderingInfo.renderArea.offset.y = 0;
			renderingInfo.renderArea.extent.width = m_desc.size.width;
			renderingInfo.renderArea.extent.height = m_desc.size.height;
			renderingInfo.layerCount = 1;
			renderingInfo.viewMask = 0;
			renderingInfo.setColorAttachments(attachment);

			commandBuffer.beginRendering(renderingInfo);

			commandBuffer.setViewport(0, vk::Viewport(0, 0, m_desc.size.width, m_desc.size.height));

			vk::Rect2D scissor;
			scissor.extent.width = m_desc.size.width;;
			scissor.extent.height = m_desc.size.height;;
			commandBuffer.setScissor(0, scissor);

			cmdList->setPipelineState(m_pipeline);

			SetDescriptorTableParam tableParam[] = {
				{m_table, 0}
			};

			cmdList->setDescriptorTables(tableParam, std::size(tableParam));

			cmdList->setVertexBuffer(m_verices);

			DrawParam drawParam;
			drawParam.startVertex = 0;
			drawParam.vertexCount = 6;
			cmdList->draw(drawParam);

			commandBuffer.endRendering();

			m_cache.clear();
			m_cache.addTexture(m_images[m_imageViews.index()], vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR, vk::ImageAspectFlagBits::eColor);
			m_cache.recordCommand(commandBuffer);

		}

	}



	void VulkanSwapChain::createBuffer() {

		// デバイスごとのサーフェイス生成
#ifdef OS_WINDOWS
		vk::Win32SurfaceCreateInfoKHR info;
		info.hinstance = GetModuleHandle(nullptr);
		info.hwnd = (HWND)m_desc.window.getHandle();

		m_surface = m_device.getInstance().createWin32SurfaceKHR(info, m_device.getAllocationCallbacks());
#elif defined(OS_LINUX)

		struct NativeWindowHandle {
			::Display* display = nullptr;
			::Window window = 0;
		};
		NativeWindowHandle* handle = reinterpret_cast<NativeWindowHandle*>(m_desc.window.getHandle());
		if (!handle) throw vk::InitializationFailedError("スワップチェーンがサポートされていません。");

		vk::XlibSurfaceCreateInfoKHR info;
		info.flags = vk::XlibSurfaceCreateFlagsKHR();
		info.dpy = handle->display;
		info.window = handle->window;
		VkXlibSurfaceCreateInfoKHR info2 = info;

		VkSurfaceKHR surface2 = VK_NULL_HANDLE;
		vkCreateXlibSurfaceKHR(*m_device.getInstance(),&info2,nullptr,&surface2);
		m_surface = vk::raii::SurfaceKHR(m_device.getInstance(), surface2);
		//m_surface = m_device.getInstance().createXlibSurfaceKHR(info, m_device.getAllocationCallbacks());
#else
		static_assert(true, "Surface is not implemented.");
#endif

		OB_ASSERT(*m_surface,"サーフェイスが生成されていません");

		// サーフェイスのサポートをチェック
		if (!m_device.getPhysicalDevice().getSurfaceSupportKHR(0, *m_surface)) {
			throw vk::InitializationFailedError("スワップチェーンがサポートされていません。");
		}

		// サーフェスの機能を取得
		auto capabilities = m_device.getPhysicalDevice().getSurfaceCapabilitiesKHR(*m_surface);
		auto surfaceFormats = m_device.getPhysicalDevice().getSurfaceFormatsKHR(*m_surface);
		auto presentModeList = m_device.getPhysicalDevice().getSurfacePresentModesKHR(*m_surface);


		vk::Extent2D size;
		size.width = std::clamp<u32>(m_desc.size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		size.height = std::clamp<u32>(m_desc.size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		if (m_desc.size.width != 0 && m_desc.size.height != 0) {
			size.width = m_desc.size.width;
			size.height = m_desc.size.height;
		}
		else if (m_desc.window) {
			size.width = m_desc.window.getSize().x;
			size.height = m_desc.window.getSize().y;
			// size = capabilities.currentExtent;
		}
		else {
			throw Exception("ウィンドウが指定されていません。");
		}

		// サイズをクランプ
		size.width = std::clamp(size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		size.height = std::clamp(size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);



		vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

		vk::SurfaceTransformFlagBitsKHR preTransform =
			(capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
			? vk::SurfaceTransformFlagBitsKHR::eIdentity
			: capabilities.currentTransform;

		vk::CompositeAlphaFlagBitsKHR compositeAlpha =
			(capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
			: (capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
			: (capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit
			: vk::CompositeAlphaFlagBitsKHR::eOpaque;

		vk::Format format = m_desc.hdr ? vk::Format::eA2R10G10B10UnormPack32 : vk::Format::eB8G8R8A8Unorm;

		Optional<vk::SurfaceFormatKHR> surfaceFormat;
		for (auto& item : surfaceFormats) {
			if (item.format == format) {
				surfaceFormat = item;
				break;
			}
		}
		if (!surfaceFormat && !surfaceFormats.empty()) {
			LOG_WARNING("サーフェスフォーマットが見つかりません。{}で代替します",magic_enum::enum_name(surfaceFormats.back().format));
			surfaceFormat = surfaceFormats.back();
		}
		if (!surfaceFormat) {
			LOG_ERROR("サーフェスフォーマットが見つかりません。");
			throw Exception();;
		}

		// サーフェイス生成
		vk::SwapchainCreateInfoKHR swapChainCreateInfo;
		swapChainCreateInfo.flags = {};
		swapChainCreateInfo.surface = *m_surface;
		swapChainCreateInfo.minImageCount = capabilities.minImageCount;
		swapChainCreateInfo.imageFormat = surfaceFormat->format;
		swapChainCreateInfo.imageColorSpace = surfaceFormat->colorSpace;
		swapChainCreateInfo.imageExtent.width = m_desc.size.width;
		swapChainCreateInfo.imageExtent.height = m_desc.size.height;
		swapChainCreateInfo.imageArrayLayers = 1;
		swapChainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		swapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
		swapChainCreateInfo.preTransform = preTransform;
		swapChainCreateInfo.compositeAlpha = compositeAlpha;
		swapChainCreateInfo.presentMode = swapchainPresentMode;
		swapChainCreateInfo.clipped = VK_TRUE;
		swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		m_swapchain = m_device.getDevice().createSwapchainKHR(swapChainCreateInfo, m_device.getAllocationCallbacks());


		// Image取得
		auto images = m_swapchain.getImages();
		m_images.insert(m_images.begin(), images.begin(), images.end());

		// ImageView生成
		for (auto [index, image] : Indexed(images)) {

			vk::ImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.flags = {};
			imageViewCreateInfo.image = image;
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = surfaceFormat->format;
			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			m_imageViews2.emplace_back(m_device.getDevice().createImageView(imageViewCreateInfo, m_device.getAllocationCallbacks()));
			m_imageViews.push_back(*m_imageViews2.back());

		}

		m_device.setName(m_surface, m_desc.name);
		m_device.setName(m_swapchain, m_desc.name);
	}

	void VulkanSwapChain::clearBuffer() {
		m_images.clear();
		m_imageViews2.clear();
		m_imageViews.clear();
		m_swapchain.clear();
		m_surface.clear();
	}


	// !@brief      applyに必要なリソースを生成
	void VulkanSwapChain::createResources() {

		{
			Vec2 vertices[] = {
				{-1,-1},
				{+1,-1},
				{-1,+1},
				{+1,-1},
				{+1,+1},
				{-1,+1},
			};
			BufferDesc bdesc = BufferDesc::Vertex<Vec2>(std::size(vertices));
			bdesc.name = m_desc.name + "_Vertices";
			m_verices = Buffer::Create(bdesc);
			m_verices->updateDirect(bdesc.size, vertices);
		}

		{
			SamplerDesc desc;
			desc.name = m_desc.name + "_Sampler";
			m_bindedSampler = m_device.createSampler(desc);
		}

		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code;
			code.append("[[vk::binding(0, 0)]]											\n");
			code.append("Texture2D g_mainTex:register(t0);								\n");
			code.append("[[vk::binding(1, 0)]]											\n");
			code.append("SamplerState g_mainSampler:register(s0);						\n");
			code.append("// IN / OUT													\n");
			code.append("struct VsIn {													\n");
			code.append("  float2 pos	:POSITION;										\n");
			code.append("};																\n");
			code.append("struct PsIn {													\n");
			code.append("  float4 pos	:SV_POSITION;									\n");
			code.append("  float2 uv	    :TEXCOORD;									\n");
			code.append("};																\n");
			code.append("// エントリ													\n");
			code.append("PsIn VS_Main(VsIn i) {											\n");
			code.append("    PsIn o;													\n");
			code.append("    o.pos = float4(i.pos*float2(2,-2)-1,0,1);				    \n");
			code.append("    o.uv = i.pos.xy;								            \n");
			code.append("    return o;													\n");
			code.append("}																\n");
			code.append("float4 PS_Main(PsIn i):SV_TARGET0{								\n");
			code.append("    return g_mainTex.Sample(g_mainSampler,i.uv);		        \n");
			code.append("}																\n");

			vs = Shader::CompileVS(code);
			ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(vs && ps);
		}

		{
			m_layout = DescriptorLayout::Create({ Binding::Texture(0),Binding::Sampler(1) });
			m_table = DescriptorTable::Create({ m_layout });
			m_table->setResource(1, m_bindedSampler);
		}

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc;
			desc.layouts = { m_layout };
			desc.name = m_desc.name;
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}

		Ref<PipelineState> pipeline;
		{
			TextureFormat format = m_desc.hdr ? TextureFormat::R10G10B10A2 : TextureFormat::RGBA8;

			PipelineStateDesc desc;
			desc.name = m_desc.name;
			desc.colors = { format };

			desc.rootSignature = signature;
			desc.vs = vs;
			desc.ps = ps;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,0,ElementType::Float,2),
			};
			desc.vertexLayout.vertexStride = sizeof(Vec2);
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
			desc.depthStencil.stencil.enable = false;

			pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(pipeline);
		}

		m_signature = signature;
		m_pipeline = pipeline;


		// m_fenceの生成
		vk::FenceCreateInfo fenceInfo;
		fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
		m_fence = m_device.getDevice().createFence(fenceInfo, m_device.getAllocationCallbacks());

		m_device.setName(m_fence, m_desc.name);
	}


	//! @brief      ウィンドウの更新イベント
	void VulkanSwapChain::onWindowChanged(const platform::WindowEventArgs& args) {

		if (args.type == platform::WindowEventType::Size || args.type == platform::WindowEventType::Maximize) {
			if (!args.isSizing) {
				if (m_desc.size.width != args.newSize.x || m_desc.size.height != args.newSize.y) {

					auto oldSize = m_desc.size;

					m_desc.size.width = (s32)args.newSize.x;
					m_desc.size.height = (s32)args.newSize.y;

					m_device.clearCommands();

					clearBuffer();
					createBuffer();

					LOG_TRACE("スワップチェーンをリサイズ ({},{}) -> ({},{})", oldSize.width, oldSize.height, m_desc.size.width, m_desc.size.height);

					m_notifier.invoke();

				}
			}
		}

		if (args.type == platform::WindowEventType::Minimize) {
			m_visible = false;
		}
		if (args.type == platform::WindowEventType::Maximize || args.type == platform::WindowEventType::Move) {
			m_visible = true;
		}

		if(args.type == platform::WindowEventType::Close) {
			m_closed = true;
		}	

	}

}
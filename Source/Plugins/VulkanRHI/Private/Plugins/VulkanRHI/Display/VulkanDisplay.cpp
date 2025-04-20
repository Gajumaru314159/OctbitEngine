//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Display/VulkanDisplay.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Sampler.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	VulkanDisplay::VulkanDisplay(VulkanRHI& rhi, const DisplayDesc& desc)
		: m_rhi(rhi)
	{
		// 未指定の場合はwindowから取得
		if (desc.size.width <= 1 || desc.size.height <= 1) {
			m_desc.size = { (s32)desc.window.getSize().x,(s32)desc.window.getSize().y };
		}

		// デバイスごとのサーフェイス生成
#ifdef OS_WINDOWS
		vk::Win32SurfaceCreateInfoKHR info;
		info.hinstance = GetModuleHandle(nullptr);
		info.hwnd = (HWND)desc.window.getHandle();

		m_surface = rhi.getInstance().createWin32SurfaceKHR(info, rhi.getAllocationCallbacks());
#else
		static_assert(true, "Surface is not implemented.");
#endif

		// サーフェイスのサポートをチェック
		if (!rhi.getPhysicalDevice().getSurfaceSupportKHR(0, m_surface)) {
			throw vk::InitializationFailedError("スワップチェーンがサポートされていません。");
		}
		
		// サーフェスの機能を取得
		auto capabilities = rhi.getPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);
		auto formats = rhi.getPhysicalDevice().getSurfaceFormatsKHR(m_surface);
		auto presentModeList = rhi.getPhysicalDevice().getSurfacePresentModesKHR(m_surface);

				
		vk::Extent2D size;
		size.width = std::clamp<u32>(desc.size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		size.height = std::clamp<u32>(desc.size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		
		if (desc.size.width != 0 && desc.size.height != 0) {
			size.width = desc.size.width;
			size.height = desc.size.height;
		} else if(desc.window) {
			size.width = desc.window.getSize().x;
			size.height = desc.window.getSize().y;
			// size = capabilities.currentExtent;
		} else {
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


		vk::SurfaceFormatKHR format = formats.at(0); // TODO desc.formatチェック

		// サーフェイス生成
		vk::SwapchainCreateInfoKHR swapChainCreateInfo;
		swapChainCreateInfo.flags = {};
		swapChainCreateInfo.surface = m_surface;
		swapChainCreateInfo.minImageCount = capabilities.minImageCount;
		swapChainCreateInfo.imageFormat = format.format;
		swapChainCreateInfo.imageColorSpace = format.colorSpace;
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

		m_swapchain = m_rhi.getDevice().createSwapchainKHR(swapChainCreateInfo, m_rhi.getAllocationCallbacks());


		// Image取得
		auto images = m_swapchain.getImages();

		// ImageView生成
		for (auto [index,image] : Indexed(images)) {

			String name = Format("{}_{}", m_desc.name,index);

			m_textures.push_back(new VulkanTexture(rhi,image,format.format, size,name));

			vk::ImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.flags = {};
			imageViewCreateInfo.image = image;
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = format.format;
			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			m_imageViews.push_back(m_rhi.getDevice().createImageView(imageViewCreateInfo, m_rhi.getAllocationCallbacks()));

			
		}

		createResources(rhi);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	VulkanDisplay::~VulkanDisplay() {
		LOG_INFO("削除");
	}


	//! @brief  定義を取得
	const DisplayDesc& VulkanDisplay::getDesc()const noexcept {
		return m_desc;
	}

	//! @brief 更新
	void VulkanDisplay::update() {
		//update(m_rhi.getQueue());
	}
	void VulkanDisplay::update(vk::Queue queue) {

		if (!m_desc.window.isValid())return;

		auto& device = m_rhi.getDevice();

		auto [result,index] = m_swapchain.acquireNextImage(1'000'000'000, {}, m_fence);
		
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

		m_textures.next();
	}


	//! @brief      イベントリスナ追加
	void VulkanDisplay::addEventListener(DisplayEventHandle& handle, DisplayEventDelegate func) {

	}


	//! @brief      テクスチャをディスプレイにコピー
	void VulkanDisplay::recordApplyDisplay(CommandList& cmdList, const Ref<RenderTexture>& texture) {

		// テクスチャが違う場合再バインド
		if (m_bindedTexture != texture) {

			m_bindedTextureTable.reset();
			m_bindedTexture = texture.get();

			if (m_bindedTexture) {
				m_bindedTextureTable = DescriptorTable::Create(m_signature, 0);
				m_bindedTextureTable->setResource(0, m_bindedTexture);
			}
			if (m_bindedSamplerTable) {
				m_bindedSamplerTable = DescriptorTable::Create(m_signature, 1);
				m_bindedSamplerTable->setResource(0, m_bindedSampler);
			}

		}

		// バインドされていなければスキップ
		if (!m_bindedTextureTable)
			return;

		{

			RenderPassDesc renderPass;
			renderPass.colors.emplace_back(m_textures.current(), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

			cmdList.beginRenderPass(renderPass);

			cmdList.setPipelineState(m_pipeline);

			SetDescriptorTableParam tableParam(m_bindedTextureTable, 0);
			cmdList.setRootDesciptorTable(&tableParam, 1);

			cmdList.setVertexBuffer(m_verices);

			DrawParam drawParam;
			drawParam.startVertex = 0;
			drawParam.vertexCount = 6;
			cmdList.draw(drawParam);

			cmdList.endRenderPass();

			// Present準備
			if (auto texture = m_textures.current().cast<VulkanTexture>()) {

				// D3D12_RESOURCE_BARRIER barrier;
				// if (texture->addResourceTransition(barrier, D3D12_RESOURCE_STATE_PRESENT)) {
				// 	cmdList.getNative()->ResourceBarrier(1, &barrier);
				// }

			}

		}
	}


	void VulkanDisplay::createResources(VulkanRHI& rhi) {

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
			m_bindedSampler = rhi.createSampler(desc);
		}

		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code;
			code.append("[[vk::binding(0, 0)]]											\n");
			code.append("Texture2D g_mainTex:register(t0);								\n");
			code.append("[[vk::binding(0, 1)]]											\n");
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

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc{
				{
					Binding::Texture(),
				},
				{
					Binding::Sampler(),
				}
			};
			desc.name = m_desc.name;
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}

		Ref<PipelineState> pipeline;
		{
			PipelineStateDesc desc;
			desc.name = m_desc.name;
			desc.colors = { m_desc.format };

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
	}

}
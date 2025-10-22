//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Plugins/VulkanRHI/Command/VulkanCommandList.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorTable.h>
#include <Plugins/VulkanRHI/PipelineState/VulkanPipelineState.h>
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>
#include <Plugins/VulkanRHI/SwapChain/VulkanSwapChain.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Plugins/VulkanRHI/VulkanDevice.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	VulkanCommandList::VulkanCommandList(VulkanDevice& device, const CommandListDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		vk::CommandPoolCreateInfo info;
		info.queueFamilyIndex = m_device.getQueryFamilyIndex();
		info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		m_commandPool = device.getDevice().createCommandPool(info, m_device.getAllocationCallbacks());

		vk::CommandBufferAllocateInfo allocInfo;
		allocInfo.commandPool = m_commandPool;
		allocInfo.commandBufferCount = 1;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;

		m_commandBuffer = std::move(m_device.getDevice().allocateCommandBuffers(allocInfo).front());
		
		m_device.setName(m_commandPool, m_desc.name);
		m_device.setName(m_commandBuffer, m_desc.name);

		manage();
	}

	//! @brief デストラクタ
	VulkanCommandList::~VulkanCommandList() {
	}

	//! @brief      名前を取得
	const String& VulkanCommandList::getName() const {
		return m_desc.name;
	}

	//! @brief  描画開始
	void VulkanCommandList::begin() {
		m_commandPool.reset();
		m_commandBuffer.reset();
		m_commandBuffer.begin({});
	}


	//! @brief  描画終了
	void VulkanCommandList::end() {
		m_commandBuffer.end();
	}


	//! @brief  描画終了
	void VulkanCommandList::flush() {
		// TODO オミット
		if (auto rhi = Device::Get()) {
			Ref<CommandList> commandList = this;
			rhi->entryCommandList(commandList);
		}
	}


	//! @brief レンダーパスを開始する 
	void VulkanCommandList::beginRenderPass(const BeginPassParam& param) {

		clearRenderTargets();

		m_cache.clear();

		s32 width = 0;
		s32 height = 0;

		FixedVector<vk::RenderingAttachmentInfo, RENDER_TARGET_MAX> colorAttachments;
		FixedVector<vk::RenderingAttachmentInfo, 1> depthAttachments;
		FixedVector<vk::RenderingAttachmentInfo, 1> stencilAttachments;

		for (auto [index,color] : Indexed(param.colors)) {
			if (auto p = color.texture.cast<VulkanTexture>()) {
				auto& attachment = colorAttachments.emplace_back();
				attachment.imageView = p->getRTV();
				attachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
				attachment.loadOp = TypeConverter::Convert(color.beforeAccess);
				attachment.storeOp = TypeConverter::Convert(color.afterAccess);
				auto fcol = p->descOfRenderTexture().clear.color;
				attachment.clearValue.color.float32 = vk::ArrayWrapper1D<float, 4>({ fcol.r,fcol.g,fcol.b,fcol.a });

				width = color.texture->width();
				height = color.texture->height();

				m_colorTextures.push_back(color.texture);

				m_cache.addTexture(p->getNative(), vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal, vk::ImageAspectFlagBits::eColor);
			} else {
				LOG_FATAL("不正な引数。レンダーテクスチャが不正です。");
			}
		}
		if (param.depth.texture) {
			if (auto p = param.depth.texture.cast<VulkanTexture>()) {
				auto& attachment = depthAttachments.emplace_back();
				attachment.imageView = p->getDSV();
				attachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
				attachment.loadOp = TypeConverter::Convert(param.depth.beforeAccess);
				attachment.storeOp = TypeConverter::Convert(param.depth.afterAccess);
				attachment.clearValue.depthStencil.depth = p->descOfRenderTexture().clear.depth;

				width = param.depth.texture->width();
				height = param.depth.texture->height();

				m_depthTexture = param.depth.texture;

				m_cache.addTexture(p->getNative(), vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthAttachmentOptimal, vk::ImageAspectFlagBits::eDepth);
			} else {
				LOG_FATAL("不正な引数。レンダーテクスチャが不正です。");
			}
		}
		if (param.stencil.texture) {
			OB_NOTIMPLEMENTED();
			if (auto p = param.stencil.texture.cast<VulkanTexture>()) {
				auto& attachment = stencilAttachments.emplace_back();
				attachment.imageView = p->getDSV();
				attachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
				attachment.loadOp = TypeConverter::Convert(param.stencil.beforeAccess);
				attachment.storeOp = TypeConverter::Convert(param.stencil.afterAccess);
				attachment.clearValue.depthStencil.stencil = p->descOfRenderTexture().clear.stencil;

				width = param.stencil.texture->width();
				height = param.stencil.texture->height();

				m_depthTexture = param.depth.texture;

				m_cache.addTexture(p->getNative(), vk::ImageLayout::eUndefined, vk::ImageLayout::eStencilAttachmentOptimal, vk::ImageAspectFlagBits::eStencil);
			}
			else {
				LOG_FATAL("不正な引数。レンダーテクスチャが不正です。");
			}
		}

		// バリア設定
		m_cache.recordCommand(m_commandBuffer);

		// 描画開始コマンド
		vk::RenderingInfo info;
		info.flags = vk::RenderingFlagBits{};
		info.renderArea.extent.width = width;
		info.renderArea.extent.height = height;
		info.layerCount = 1;
		info.viewMask = 0;
		if (!colorAttachments.empty()) {
			info.setColorAttachments(colorAttachments);
		}
		if (!depthAttachments.empty()) {
			info.pDepthAttachment = depthAttachments.data();
		}
		if (!stencilAttachments.empty()) {
			info.pStencilAttachment = stencilAttachments.data();
		}
		m_commandBuffer.beginRendering(info);

		// 初期設定としてViewportとScissorRectを設定
		Viewport viewport(0, 0, 1.0f * width, 1.0f * height);
		IntRect scissor(0, 0, 1.0f * width, 1.0f * height);

		setViewport(&viewport, 1);
		setScissorRect(&scissor, 1);

	}


	//! @brief 	レンダーパスを終了する
	void VulkanCommandList::endRenderPass() {
		
		m_commandBuffer.endRendering();

		m_cache.clear();
		
		for (auto [i, color] : Indexed(m_colorTextures)) {
			if (auto texture = color.cast<VulkanTexture>()) {
				m_cache.addTexture(texture->getNative(),vk::ImageLayout::eColorAttachmentOptimal,vk::ImageLayout::eShaderReadOnlyOptimal,vk::ImageAspectFlagBits::eColor);
			}
		}
		if (auto texture = m_depthTexture.cast<VulkanTexture>()) {
			m_cache.addTexture(texture->getNative(), vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::ImageLayout::eGeneral, vk::ImageAspectFlagBits::eDepth);
		}
		
		// リソースバリア
		m_cache.recordCommand(m_commandBuffer);

		clearRenderTargets();
	}


	//! @brief レンダーターゲットをクリア
	void VulkanCommandList::clearRenderTargets() {
		m_colorTextures.clear();
		m_depthTexture = nullptr;
	}


	//! @brief      スワップチェーンにテクスチャを適用
	void VulkanCommandList::applySwapChain(const Ref<SwapChain>& swapChain, const Ref<RenderTexture>& texture) {
		OB_ASSERT_EXPR(m_commandBuffer != nullptr);
		if (auto pSwapChain = swapChain.cast<VulkanSwapChain>()) {
			Ref<CommandList> commandList = this;
			pSwapChain->recordApplySwapChain(commandList, texture);
		}
		else {
			LOG_ERROR("不正な引数。スワップチェーンが不正です。");
		}
	}


	//! @brief  シザー矩形を設定
	void VulkanCommandList::setScissorRect(const IntRect* pRect, s32 num) {

		FixedVector<vk::Rect2D, RENDER_TARGET_MAX> rects;
		for (s32 i = 0; i < num; ++i) {
			auto& rectIn = pRect[i];
			auto& rectOut = rects.emplace_back();
			rectOut.offset.x = rectIn.left;
			rectOut.offset.y = rectIn.top;
			rectOut.extent.width = rectIn.width();
			rectOut.extent.height = rectIn.height();
		}

		m_commandBuffer.setScissor(0, rects);

	}


	//! @brief  ビューポートを設定
	void VulkanCommandList::setViewport(const Viewport* pViewport, s32 num) {

		FixedVector<vk::Viewport, RENDER_TARGET_MAX> viewports;
		for (s32 i = 0; i < num;++i) {
			auto& viewportIn = pViewport[i];
			auto& viewportOut = viewports.emplace_back();
			viewportOut.x = viewportIn.top;
			viewportOut.y = viewportIn.left;
			viewportOut.width = viewportIn.width();
			viewportOut.height = viewportIn.height();
			viewportOut.minDepth = viewportIn.nearZ;
			viewportOut.maxDepth = viewportIn.farZ;
		}

		m_commandBuffer.setViewport(0, viewports);

	}


	//! @brief      頂点バッファを設定
	void VulkanCommandList::setVertexBuffers(Span<Ref<Buffer>> buffers, s32 first) {

		FixedVector<vk::Buffer, VERTEX_BUFFER_MAX> vkBuffers;
		FixedVector<vk::DeviceSize, VERTEX_BUFFER_MAX> offsets;

		for (auto& buffer : buffers) {
			if (auto p = buffer.cast<VulkanBuffer>()) {
				vkBuffers.push_back(p->getNative());
				offsets.push_back(0);
			} else {
				LOG_FATAL("不正な引数。頂点バッファが不正です。");
			}
		}

		m_commandBuffer.bindVertexBuffers(first,vkBuffers, offsets);
	}


	//! @brief      インデックスバッファを設定
	void VulkanCommandList::setIndexBuffer(const Ref<Buffer>& buffer) {

		if (auto p = buffer.cast<VulkanBuffer>()) {

			Optional<vk::IndexType> type;

			s32 stride = buffer->getDesc().stride;
			if (stride == 2) type = vk::IndexType::eUint16;
			if (stride == 4) type = vk::IndexType::eUint32;
			if (!type) LOG_FATAL("インデックスバッファが不正なストライド幅です。16bitか32bitから選択してください。");

			m_commandBuffer.bindIndexBuffer(p->getNative(), 0, type.value());
		} else {
			LOG_FATAL("不正な引数。インデックスバッファが不正です。");
		}

	}


	//! @brief      パイプラインステートを設定
	void VulkanCommandList::setPipelineState(const Ref<PipelineState>& pipeline) {

		if (auto p = pipeline.cast<VulkanPipelineState>()) {
			m_pipeline = pipeline;
			m_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, p->getNative());

			auto& desc = p->getDesc().rootSignature->getDesc();
			if (desc.flags.has(RootSignatureFlag::EnableBindless)) {
				s32 bindlessSlot = gsl::narrow<s32>(desc.layouts.size());
				m_device.setDescriptorHeaps(m_commandBuffer,p->getLayout(), bindlessSlot);			
			}
		} else {
			LOG_FATAL("不正な引数。パイプラインステートが不正です。");
		}

	}


	//! @brief      描画
	void VulkanCommandList::draw(const DrawParam& param) {
		m_commandBuffer.draw(param.vertexCount, param.instanceCount, param.startVertex, param.startInstance);
	}


	//! @brief      インデックス描画
	void VulkanCommandList::drawIndexed(const DrawIndexedParam& param) {
		m_commandBuffer.drawIndexed(param.indexCount,param.instanceCount, param.startIndex,gsl::narrow_cast<int32_t>(param.startVertex),param.startInstance);
	}


	//! @brief      デスクリプタテーブルを設定
	void VulkanCommandList::setDescriptorTables(const rhi::SetDescriptorTableParam* params, s32 num) {

		auto pipeline = m_pipeline.cast<VulkanPipelineState>();
		OB_ASSERT(pipeline != nullptr, "先にPipelineStateを設定してください");
		auto layout = pipeline->getLayout();

		for (s32 i = 0; i < num; ++i) {
			auto& param = params[i];

			if (auto impl = param.table.cast<VulkanDescriptorTable>()) {
				impl->record(m_commandBuffer, layout, param.slot);
			} else {
				LOG_FATAL("不正な引数。デスクリプタテーブルが不正です。");
			}

		}

	}


	//! @brief      ルート定数を設定
	void VulkanCommandList::setRootConstant(const SetRootConstantsParam& param) {

		if (auto pipeline = m_pipeline.cast<VulkanPipelineState>()) {
			if (auto signature = pipeline->getDesc().rootSignature.cast<VulkanRootSignature>()) {
				m_commandBuffer.pushConstants<byte>(signature->getNative(), vk::ShaderStageFlags{} | vk::ShaderStageFlagBits::eAll,param.offset, param.blob);
				return;
			}
		}

		OB_ASSERT(m_pipeline, "PipelineStateが未設定です。");
	}


	//! @brief      GPUマーカーをプッシュ
	void VulkanCommandList::pushMarker(StringView name) {
		if (m_device.getFeaturesEx().debugMarkerEnabled) m_commandBuffer.beginDebugUtilsLabelEXT(name.data());
	}


	//! @brief      GPUマーカーをポップ
	void VulkanCommandList::popMarker() {
		if (m_device.getFeaturesEx().debugMarkerEnabled) m_commandBuffer.endDebugUtilsLabelEXT();
	}
	
}
//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "VulkanCommandList.h"
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Display/VulkanDisplay.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>
#include <Plugins/VulkanRHI/PipelineState/VulkanPipelineState.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorTable.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	VulkanCommandList::VulkanCommandList(VulkanRHI& rhi, const CommandListDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{
		vk::CommandPoolCreateInfo info;
		info.queueFamilyIndex = m_rhi.getQueryFamilyIndex();
		info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		m_commandPool = rhi.getDevice().createCommandPool(info, m_rhi.getAllocationCallbacks());

		vk::CommandBufferAllocateInfo allocInfo;
		allocInfo.commandPool = m_commandPool;
		allocInfo.commandBufferCount = 1;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;

		m_commandBuffer = std::move(m_rhi.getDevice().allocateCommandBuffers(allocInfo).front());
	}

	VulkanCommandList::~VulkanCommandList() {
	}

	//! @brief      名前を取得
	const String& VulkanCommandList::getName() const {
		return m_desc.name;
	}

#pragma endregion Command

	//@―---------------------------------------------------------------------------
	//! @brief  描画開始
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::begin() {
		m_commandPool.reset(vk::CommandPoolResetFlags{});
		m_commandBuffer.reset(vk::CommandBufferResetFlags{});
		vk::CommandBufferBeginInfo info;
		//info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		m_commandBuffer.begin(info);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::end() {
		m_commandBuffer.end();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::flush() {
		if (auto rhi = RHI::Get()) {
			Ref<CommandList> commandList = this;
			rhi->entryCommandList(commandList);
		}
	}

	void VulkanCommandList::beginRenderPass(const RenderPassDesc& param) {

		m_colorTextures.clear();
		m_depthTexture = nullptr;

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

				width = color.texture->width();
				height = color.texture->height();

				m_colorTextures.push_back(color.texture);

				m_cache.addTexture(p->getNative(), vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal, vk::ImageAspectFlagBits::eColor);
			} else {
				LOG_ERROR("不正な引数。レンダーターゲットが不正です。");
			}
		}
		if (param.depth.texture) {
			if (auto p = param.depth.texture.cast<VulkanTexture>()) {
				auto& attachment = depthAttachments.emplace_back();
				attachment.imageView = p->getDSV();
				attachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
				attachment.loadOp = TypeConverter::Convert(param.depth.beforeAccess);
				attachment.storeOp = TypeConverter::Convert(param.depth.afterAccess);

				// TODO Stencilの扱い

				width = param.depth.texture->width();
				height = param.depth.texture->height();

				m_depthTexture = param.depth.texture;

				m_cache.addTexture(p->getNative(), vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthAttachmentOptimal, vk::ImageAspectFlagBits::eDepth);
			} else {
				LOG_ERROR("不正な引数。レンダーターゲットが不正です。");
			}
		}
		if (param.stencil.texture) {
			if (auto p = param.stencil.texture.cast<VulkanTexture>()) {
				auto& attachment = stencilAttachments.emplace_back();
				attachment.imageView = p->getDSV();
				attachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
				attachment.loadOp = TypeConverter::Convert(param.stencil.beforeAccess);
				attachment.storeOp = TypeConverter::Convert(param.stencil.afterAccess);

				// TODO Stencilの扱い

				width = param.stencil.texture->width();
				height = param.stencil.texture->height();

				m_depthTexture = param.depth.texture;

				m_cache.addTexture(p->getNative(), vk::ImageLayout::eUndefined, vk::ImageLayout::eStencilAttachmentOptimal, vk::ImageAspectFlagBits::eStencil);
			}
			else {
				LOG_ERROR("不正な引数。レンダーターゲットが不正です。");
			}
		}

		vk::RenderingInfo info;
		info.flags = vk::RenderingFlagBits{};
		info.renderArea.offset.x = 0;
		info.renderArea.offset.y = 0;
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

		m_cache.recordCommand(m_commandBuffer);

		m_commandBuffer.beginRendering(info);

		// 初期設定としてViewportとScissorRectを設定
		FixedVector<Viewport, RENDER_TARGET_MAX> viewports;
		FixedVector<IntRect, RENDER_TARGET_MAX> scissors;
		for (s32 i = 0; i < std::max<s32>(param.colors.size(),1);++i) {
			viewports.emplace_back(0, 0, width, height);
			scissors.emplace_back(0, 0, width, height);
		}

		setViewport(viewports.data(), (UINT)viewports.size());
		setScissorRect(scissors.data(), (UINT)scissors.size());

	}

	void VulkanCommandList::endRenderPass() {
		
		m_commandBuffer.endRendering();

		m_cache.clear();
		
		for (auto [i, color] : Indexed(m_colorTextures)) {
			if (auto texture = color.cast<VulkanTexture>()) {
				m_cache.addTexture(texture->getNative(),vk::ImageLayout::eColorAttachmentOptimal,vk::ImageLayout::eShaderReadOnlyOptimal,vk::ImageAspectFlagBits::eColor);
			}
		}
		if (auto texture = m_depthTexture.cast<VulkanTexture>()) {
			m_cache.addTexture(texture->getNative(), vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::ImageLayout::eGeneral, vk::ImageAspectFlagBits::eColor);
		}
		
		// リソースバリア
		m_cache.recordCommand(m_commandBuffer);

		m_colorTextures.clear();
		m_depthTexture = nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      スワップチェーンにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture) {
		OB_ASSERT_EXPR(m_commandBuffer != nullptr);
		if (auto pDisplay = display.cast<VulkanDisplay>()) {
			Ref<CommandList> commandList = this;
			pDisplay->recordApplyDisplay(commandList, texture);
		}
		else {
			LOG_ERROR("不正な引数。ディスプレイが不正です。");
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  シザー矩形を設定
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::setScissorRect(const IntRect* pRect, s32 num) {

		FixedVector<vk::Rect2D, 8> rects;
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


	//@―---------------------------------------------------------------------------
	//! @brief  ビューポートを設定
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::setViewport(const Viewport* pViewport, s32 num) {

		OB_ASSERT_EXPR(m_commandBuffer != nullptr);

		FixedVector<vk::Viewport, 8> viewports;
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


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::clearColors(u32 mask) {
		// for (auto [i, texture] : Indexed(m_colorTextures)) {
		// 	if (!(mask & (1 << i)))continue;
		// 	if (auto impl = texture.cast<VulkanTexture>()) {
		// 		Color color = texture->descOfRenderTexture().clear.color;
		// 		vk::ClearColorValue value(color.r, color.g, color.b, color.a);
		// 
		// 		m_commandBuffer.clearColorImage(impl->getNative(), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eColorAttachmentOptimal, vk::ClearColorValue{0.0f,0.0f,0.0f,1.0f}, {0, 0, 1});
		// 	}
		// }
		//m_commandBuffer.clearColorImage(m_renderTarget->getNative(), vk::ImageLayout::eColorAttachmentOptimal, vk::ClearColorValue{ 0.0f,0.0f,0.0f,1.0f }, { 0, 0, 1 });
	}


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットのデプスとステンシルをクリア
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::clearDepthStencil() {

		//OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      頂点バッファを設定
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::setVertexBuffers(Span<Ref<Buffer>> buffers) {
		OB_ASSERT_EXPR(m_commandBuffer != nullptr);
		FixedVector<vk::Buffer, 8> vkBuffers;
		FixedVector<vk::DeviceSize, 8> offsets;
		for (auto& buffer : buffers) {
			if (auto p = buffer.cast<VulkanBuffer>()) {
				vkBuffers.push_back(p->getNative());
				offsets.push_back(0);
			}
			else {
				LOG_ERROR("不正な引数。頂点バッファが不正です。");
			}
		}
		m_commandBuffer.bindVertexBuffers(0,vkBuffers, offsets);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックスバッファを設定
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::setIndexBuffer(const Ref<Buffer>& buffer) {
		OB_ASSERT_EXPR(m_commandBuffer != nullptr);
		if (auto p = buffer.cast<VulkanBuffer>()) {
			vk::IndexType indexType = vk::IndexType::eUint16;
			//TODO 16チェック
			if (p->getDesc().stride ==sizeof(u32)) {
				indexType = vk::IndexType::eUint32;
			}
			m_commandBuffer.bindIndexBuffer(p->getNative(), 0, indexType);
		}
		else {
			LOG_ERROR("不正な引数。インデックスバッファが不正です。");
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      パイプラインステートを設定
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::setPipelineState(const Ref<PipelineState>& pipeline) {
		OB_ASSERT_EXPR(m_commandBuffer != nullptr);
		if (auto p = pipeline.cast<VulkanPipelineState>()) {
			m_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, p->getNative());
		}
		else {
			LOG_ERROR("不正な引数。パイプラインステートが不正です。");
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::draw(const DrawParam& param) {
		m_commandBuffer.draw(param.vertexCount, 1, param.startVertex, 0);

	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックス描画
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::drawIndexed(const DrawIndexedParam& param) {
		m_commandBuffer.draw(param.indexCount,param.indexCount, param.startVertex, param.startIndex);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      デスクリプタテーブルを設定
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::setRootDesciptorTable(const rhi::SetDescriptorTableParam* params, s32 num) {

		for (s32 i = 0; i < num; ++i) {
			auto& param = params[i];

			if (auto impl = param.table.cast<VulkanDescriptorTable>()) {
				impl->record(m_commandBuffer, param.slot);
			} else {
				LOG_ERROR("不正な引数。デスクリプタテーブルが不正です。");
			}

		}

	}


	//! @brief      ルート定数を設定
	void VulkanCommandList::setRootConstant(const SetRootConstantsParam& param) {
		// m_commandBuffer.pushConstants();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  リソースバリアを挿入
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::insertResourceBarrier(const ResourceBarrier& resourceBarrier) {

		OB_NOTIMPLEMENTED();
	}

	//! @brief      GPUマーカーをプッシュ
	void VulkanCommandList::pushMarker(StringView name) {
		// if (m_rhi.debugMarkerEnabled) m_commandBuffer.debugMarkerBeginEXT("BufferUploader");
	}

	//! @brief      GPUマーカーをポップ
	void VulkanCommandList::popMarker() {
		// if (m_rhi.debugMarkerEnabled) m_commandBuffer.debugMarkerEndEXT();
	}

#pragma endregion

}
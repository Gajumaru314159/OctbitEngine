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
		m_commandBuffer.begin(vk::CommandBufferBeginInfo{});
	}


	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::end() {
		m_commandBuffer.end();
		m_commandPool.reset(vk::CommandPoolResetFlags{});
		m_commandBuffer.reset(vk::CommandBufferResetFlags{});
	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::flush() {
		if (auto rhi = RHI::Get()) {
			rhi->entryCommandList(*this);
		}
	}

	//! @brief      描画先設定
	void VulkanCommandList::setRenderTargets(const RenderTextureArray& colors, const Ref<RenderTexture>& depth) {

		FixedVector<vk::RenderingAttachmentInfo, 8> colorAttachments;
		FixedVector<vk::RenderingAttachmentInfo, 1> depthAttachments;
		for (s32 i = 0; i < colors.size(); ++i) {
			if (auto p = colors[i].cast<VulkanTexture>()) {
				auto& colorAttachment = colorAttachments.emplace_back();
				colorAttachment.imageView = p->getRTV();
				colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
			}
			else {
				LOG_ERROR("不正な引数。レンダーターゲットが不正です。");
			}
		}
		{
			if (auto p = depth.cast<VulkanTexture>()) {
				auto& depthAttachment = depthAttachments.emplace_back();
				depthAttachment.imageView = p->getRTV();
				depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
			}
			else {
				LOG_ERROR("不正な引数。レンダーターゲットが不正です。");
			}
		}

		vk::RenderingInfo info;
		info.flags = vk::RenderingFlagBits{};
		info.renderArea.offset.x = 0;
		info.renderArea.offset.y = 0;
		info.renderArea.extent.width = colors[0]->width();
		info.renderArea.extent.height = colors[0]->height();
		info.layerCount = 1;
		info.viewMask = 0;
		if (!colorAttachments.empty()) {
			info.pColorAttachments = colorAttachments.data();
			info.colorAttachmentCount = colorAttachments.size();
		}
		if (!depthAttachments.empty()) {
			info.pDepthAttachment = depthAttachments.data();
		}
		//if (!stencilAttachments.empty()) {
		//	info.pStencilAttachment = stencilAttachments.data();
		//}

		m_commandBuffer.beginRendering(info);

		// TODO endRenderingはどう呼ぶ？
	}

	//@―---------------------------------------------------------------------------
	//! @brief      スワップチェーンにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture) {
		OB_ASSERT_EXPR(m_commandBuffer != nullptr);
		if (auto p = display.cast<VulkanDisplay>()) {
			OB_NOTIMPLEMENTED();
			//p->present(texture, m_commandBuffer);
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
			auto& rectOut = rects[i];
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
		//OB_NOTIMPLEMENTED();
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

		OB_NOTIMPLEMENTED();
	}


	//! @brief      ルート定数を設定
	void VulkanCommandList::setRootConstant(const SetRootConstantsParam&) {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  リソースバリアを挿入
	//@―---------------------------------------------------------------------------
	void VulkanCommandList::insertResourceBarrier(const ResourceBarrier& resourceBarrier) {

		OB_NOTIMPLEMENTED();
	}

	//! @brief      GPUマーカーをプッシュ
	void VulkanCommandList::pushMarker(StringView name) {

	}

	//! @brief      GPUマーカーをポップ
	void VulkanCommandList::popMarker() {

	}

#pragma endregion

}
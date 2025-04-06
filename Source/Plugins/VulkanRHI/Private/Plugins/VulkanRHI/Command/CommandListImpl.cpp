//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "CommandListImpl.h"
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Display/DisplayImpl.h>
//#include <Plugins/VulkanRHI/Texture/RenderTargetImpl.h>
//#include <Plugins/VulkanRHI/Texture/TextureImpl.h>
//#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/VulkanRHI/PipelineState/PipelineStateImpl.h>
//#include <Plugins/VulkanRHI/Descriptor/DescriptorTableImpl.h>
//#include <Plugins/VulkanRHI/Texture/RenderTargetImpl.h>
//#include <Plugins/VulkanRHI/Buffer/BufferImpl.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandListImpl::CommandListImpl(const CommandListDesc& desc, VkDevice device, u32 queueFamilyIndex)
		: m_desc(desc)
		, m_device(device)
	{
		VkResult result;

		VkCommandPoolCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		ci.queueFamilyIndex = queueFamilyIndex;
		ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		result = vkCreateCommandPool(device, &ci, nullptr, &m_commandPool);
		if (result == VK_SUCCESS) {
			return;
		}

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = m_commandPool;
		info.commandBufferCount;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		result = vkAllocateCommandBuffers(m_device, &info, &m_commandBuffer);
		if (result == VK_SUCCESS) {
			return;
		}

	}

	CommandListImpl::~CommandListImpl() {
		if (m_commandBuffer) {
			// TODO はきがひつようかしらべる
			m_commandBuffer = nullptr;
		}
		if (m_commandPool) {
			::vkDestroyCommandPool(m_device, m_commandPool, nullptr);
			m_commandPool = nullptr;
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool CommandListImpl::isValid()const {
		return false;
	}

	//! @brief      名前を取得
	const String& CommandListImpl::getName() const {
		return m_desc.name;
	}

#pragma endregion Command

	//@―---------------------------------------------------------------------------
	//! @brief  描画開始
	//@―---------------------------------------------------------------------------
	void CommandListImpl::begin() {
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::end() {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::flush() {
		OB_NOTIMPLEMENTED();
	}

	//! @brief      描画先設定
	void CommandListImpl::setRenderTargets(const RenderTextureArray& colors, const Ref<RenderTexture>& depth) {

	}

	//@―---------------------------------------------------------------------------
	//! @brief      スワップチェーンにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void CommandListImpl::applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture) {
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  シザー矩形を設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setScissorRect(const IntRect* pRect, s32 num) {

		OB_ASSERT_EXPR(m_commandBuffer != nullptr);

		Array<VkRect2D, 8> rects;
		for (s32 i = 0; i < num; ++i) {
			auto& rectIn = pRect[i];
			auto& rectOut = rects[i];
			rectOut.offset.x = rectIn.left;
			rectOut.offset.y = rectIn.top;
			rectOut.extent.width = rectIn.width();
			rectOut.extent.height = rectIn.height();
		}

		::vkCmdSetScissor(m_commandBuffer, 0, num, rects.data());

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ビューポートを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setViewport(const Viewport* pViewport, s32 num) {

		OB_ASSERT_EXPR(m_commandBuffer != nullptr);

		Array<VkViewport, 8> viewports;
		for (s32 i = 0; i < num;++i) {
			auto& viewportIn = pViewport[i];
			auto& viewportOut = viewports[i];
			viewportOut.x = viewportIn.top;
			viewportOut.y = viewportIn.left;
			viewportOut.width = viewportIn.width();
			viewportOut.height = viewportIn.height();
			viewportOut.minDepth = viewportIn.nearZ;
			viewportOut.maxDepth = viewportIn.farZ;
		}

		::vkCmdSetViewport(m_commandBuffer, 0, num, viewports.data());

	}


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearColors(u32 mask) {

		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットのデプスとステンシルをクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearDepthStencil() {

		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      頂点バッファを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setVertexBuffers(Span<Ref<Buffer>> buffers) {

		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックスバッファを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setIndexBuffer(const Ref<Buffer>& buffer) {

		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      パイプラインステートを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setPipelineState(const Ref<PipelineState>&) {

		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void CommandListImpl::draw(const DrawParam& param) {

		OB_ASSERT_EXPR(m_commandBuffer);
		::vkCmdDraw(m_commandBuffer, param.vertexCount, 0, param.startVertex, 0);

	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックス描画
	//@―---------------------------------------------------------------------------
	void CommandListImpl::drawIndexed(const DrawIndexedParam& param) {

		OB_ASSERT_EXPR(m_commandBuffer);
		::vkCmdDrawIndexed(m_commandBuffer, param.indexCount, 0, param.startIndex,param.startVertex, 0);

	}


	//@―---------------------------------------------------------------------------
	//! @brief      デスクリプタテーブルを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setRootDesciptorTable(const rhi::SetDescriptorTableParam* params, s32 num) {

		OB_NOTIMPLEMENTED();
	}


	//! @brief      ルート定数を設定
	void CommandListImpl::setRootConstant(const SetRootConstantsParam&) {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  リソースバリアを挿入
	//@―---------------------------------------------------------------------------
	void CommandListImpl::insertResourceBarrier(const ResourceBarrier& resourceBarrier) {

		OB_NOTIMPLEMENTED();
	}

	//! @brief      GPUマーカーをプッシュ
	void CommandListImpl::pushMarker(StringView name) {

	}

	//! @brief      GPUマーカーをポップ
	void CommandListImpl::popMarker() {

	}

#pragma endregion

}
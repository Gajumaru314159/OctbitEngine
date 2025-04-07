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
	CommandListImpl::CommandListImpl(VulkanRHI& rhi, const CommandListDesc& desc)
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

	CommandListImpl::~CommandListImpl() {
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

		Array<vk::Rect2D, 8> rects;
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
	void CommandListImpl::setViewport(const Viewport* pViewport, s32 num) {

		OB_ASSERT_EXPR(m_commandBuffer != nullptr);

		Array<vk::Viewport, 8> viewports;
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

		m_commandBuffer.setViewport(0, viewports);

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
		m_commandBuffer.draw(param.vertexCount, 1, param.startVertex, 0);

	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックス描画
	//@―---------------------------------------------------------------------------
	void CommandListImpl::drawIndexed(const DrawIndexedParam& param) {
		m_commandBuffer.draw(param.indexCount,param.indexCount, param.startVertex, param.startIndex);
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
//***********************************************************
//! @file
//! @brief		コマンドリスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/CommandList.h>
#include <Framework/Graphic/Interface/ICommandList.h>
#include <Framework/Graphic/Device.h>
#include <Framework/Graphic/Types/CommandListDesc.h>

namespace ob::graphic {

	OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(CommandList);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のCommandListへアクセス")

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandList::CommandList() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandList::CommandList(const CommandListDesc& desc) {
		m_pImpl = Device::Get()->createCommandList(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		if (!m_pImpl->isValid()) {
			LOG_FATAL_EX("Graphic", "CommandListの生成に失敗");
			release();
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドをシステムキューに追加
	//@―---------------------------------------------------------------------------
	void CommandList::flush() {
		Device::Get()->entryCommandList(*this);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      記録開始
	//@―---------------------------------------------------------------------------
	void CommandList::begin() {
		CHECK_IMPL();
		m_pImpl->begin();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      記録終了
	//@―---------------------------------------------------------------------------
	void CommandList::end() {
		CHECK_IMPL();
		m_pImpl->end();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      スワップチェーンにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void CommandList::applySwapChain(const class SwapChain& swapChain, const class Texture& texture)
	{
		CHECK_IMPL();
		m_pImpl->applySwapChain(swapChain,texture);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画開始
	//@―---------------------------------------------------------------------------
	void CommandList::beginRender(const RenderTarget& target) {
		CHECK_IMPL();
		m_pImpl->beginRender(target);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画終了
	//@―---------------------------------------------------------------------------
	void CommandList::endRender() {
		CHECK_IMPL();
		m_pImpl->endRender();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シザー矩形を設定
	//@―---------------------------------------------------------------------------
	void CommandList::setScissorRect(const IntRect* pRect, s32 num) {
		CHECK_IMPL();
		m_pImpl->setScissorRect(pRect, num);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setViewport(const Viewport* pViewport, s32 num) {
		CHECK_IMPL();
		m_pImpl->setViewport(pViewport, num);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
	//@―---------------------------------------------------------------------------
	void CommandList::clearColors(u32 mask) {
		CHECK_IMPL();
		m_pImpl->clearColors(mask);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットのデプスとステンシルをクリア
	//@―---------------------------------------------------------------------------
	void CommandList::clearDepthStencil() {
		CHECK_IMPL();
		m_pImpl->clearDepthStencil();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      ルートシグネチャを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setRootSignature(const class RootSignature& signature) {
		CHECK_IMPL();
		m_pImpl->setRootSignature(signature);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画パイプラインを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setPipelineState(const class PipelineState& pipeline) {
		CHECK_IMPL();
		m_pImpl->setPipelineState(pipeline);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      頂点バッファを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setVertexBuffer(const Buffer& buffer) {
		CHECK_IMPL();
		// TODO パラメータ指定
		StaticArray<const Buffer*, 1> buffers = {
			&buffer
		};
		const Buffer* pBuffer = &buffer;
		setVertexBuffers(buffers);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      頂点バッファを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setVertexBuffers(Span<const Buffer*> buffers) {
		CHECK_IMPL();
		// TODO パラメータ指定
		m_pImpl->setVertexBuffers(buffers);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      インデックスバッファを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setIndexBuffer(const Buffer& buffer) {
		CHECK_IMPL();
		m_pImpl->setIndexBuffer(buffer);
	}

	void CommandList::setRootDesciptorTable(const SetDescriptorTableParam* params, s32 num) {
		CHECK_IMPL();
		m_pImpl->setRootDesciptorTable(params,num);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void CommandList::draw(const DrawParam& param) {
		CHECK_IMPL();
		m_pImpl->draw(param);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックス描画
	//@―---------------------------------------------------------------------------
	void CommandList::drawIndexed(const DrawIndexedParam& param) {
		CHECK_IMPL();
		m_pImpl->drawIndexed(param);
	}

}// namespace ob
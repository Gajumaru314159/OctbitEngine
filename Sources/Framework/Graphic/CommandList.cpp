﻿//***********************************************************
//! @file
//! @brief		コマンドリスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/CommandList.h>
#include <Framework/Graphic/Interface/ICommandList.h>
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Types/CommandListDesc.h>
#include <Framework/Graphic/VertexBuffer.h>
#include <Framework/Graphic/IndexBuffer.h>

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
	CommandList::CommandList(const CommandListDesc& desc, StringView name) {
		m_pImpl = Device::Get()->createCommandList(desc);
		OB_CHECK_ASSERT_EXPR(m_pImpl);
		m_pImpl->setName(name);
		if (!m_pImpl->isValid()) {
			LOG_ERROR_EX("Graphic", "CommandListの生成に失敗 [name={}]", name);
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
	//! @brief      書き込み先をスワップチェーンに設定
	//@―---------------------------------------------------------------------------
	void CommandList::setRenderTarget(const SwapChain& target) {
		CHECK_IMPL();
		m_pImpl->setRenderTarget(target);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットを設定
	//@―---------------------------------------------------------------------------
	void CommandList::setRenderTarget(const RenderTarget& target) {
		CHECK_IMPL();
		m_pImpl->setRenderTarget(target);
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
		m_pImpl->setVertexBuffer(buffer);
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
	//! @brief      インデックスありで描画
	//@―---------------------------------------------------------------------------
	void CommandList::drawIndexedInstanced() {
		CHECK_IMPL();
		m_pImpl->drawIndexed();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      インデックスなしで描画
	//@―---------------------------------------------------------------------------
	void CommandList::drawInstanced() {
		OB_NOTIMPLEMENTED();
	}

}// namespace ob
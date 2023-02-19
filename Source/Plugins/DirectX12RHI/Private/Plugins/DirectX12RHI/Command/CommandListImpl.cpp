//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "CommandListImpl.h"
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Display/DisplayImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/DirectX12RHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorTableImpl.h>
#include <Plugins/DirectX12RHI/RenderPass/RenderPassImpl.h>
#include <Plugins/DirectX12RHI/FrameBuffer/FrameBufferImpl.h>
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

#include <pix3.h>

//===============================================================
// # CommandListのマルチスレッド対応について
// * https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/recording-command-lists-and-bundles#recording-command-lists
// * https://shobomaru.wordpress.com/2015/04/20/d3d12-command/
// * https://shobomaru.wordpress.com/2015/07/12/d3d12-fence/
//===============================================================


namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandListImpl::CommandListImpl(class DeviceImpl& device, const CommandListDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		HRESULT result;
		auto type = TypeConverter::Convert(desc.type);

		// アロケータ生成
		result = device.getNative()->CreateCommandAllocator(type, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommandAllocator()"));
			return;
		}

		// コマンドリスト生成
		UINT nodeMask = 0;
		result = device.getNative()->CreateCommandList(nodeMask, type, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommandList()"));
			return;
		}

		// 初期状態で記録モードなのでクローズ
		m_cmdList->Close();

		Utility::setName(m_cmdAllocator.Get(), getName());
		Utility::setName(m_cmdList.Get(), getName());
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool CommandListImpl::isValid()const {
		return m_cmdList;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      名前を取得
	//@―---------------------------------------------------------------------------
	const String& CommandListImpl::getName()const {
		return m_desc.name;
	}

#pragma endregion Command

	//@―---------------------------------------------------------------------------
	//! @brief  描画開始
	//@―---------------------------------------------------------------------------
	void CommandListImpl::begin() {

		HRESULT result;

		clearDescriptorHandle();

		// コマンドアロケータをリセット
		result = m_cmdAllocator->Reset();
		if (FAILED(result)) {
			String message = TC("ID3D12CommandAllocator::Reset()\n");
			message += Utility::getDebugLayerLastString(m_device.getNative().Get());
			Utility::OutputFatalLog(result, message);
		}

		// コマンドリストをリセット
		result = m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
		if (FAILED(result)) {
			String message = TC("ID3D12CommandList::Reset()\n");
			message += Utility::getDebugLayerLastString(m_device.getNative().Get());
			Utility::OutputFatalLog(result, message);
		}

		Utility::setName(m_cmdAllocator.Get(), getName());
		Utility::setName(m_cmdList.Get(), getName());

		// デスクリプタヒープを設定
		m_device.setDescriptorHeaps(*this);

		m_rootSignature = nullptr;
		m_frameBuffer = nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::end() {
		m_cmdList->Close();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  コマンドをシステムキューに追加
	//@―---------------------------------------------------------------------------
	void CommandListImpl::flush() {
		Device::Get()->entryCommandList(*this);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーパス開始
	//@―---------------------------------------------------------------------------
	void CommandListImpl::beginRenderPass(const Ref<FrameBuffer>& frameBuffer) {

		m_frameBuffer = frameBuffer.get();
		m_subpassIndex = 0;

		setSubpass();

	}

	//@―---------------------------------------------------------------------------
	//! @brief      サブパス設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setSubpass() {
		
		if (!m_frameBuffer)return;

		auto& frameBufferDesc = m_frameBuffer->desc();
		auto& attachments = frameBufferDesc.attachments;
		auto& renderPassDesc = frameBufferDesc.renderPass->desc();

		if (!is_in_range(m_subpassIndex, renderPassDesc.subpasses)){
			LOG_WARNING("サブパスが範囲外です。[index={},size={}]",m_subpassIndex, renderPassDesc.subpasses.size());
			return;
		}

		auto& subpass = renderPassDesc.subpasses[m_subpassIndex];
		auto& attachmentDescs = renderPassDesc.attachments;

		D3D12_CPU_DESCRIPTOR_HANDLE colors[8]{};
		D3D12_CPU_DESCRIPTOR_HANDLE depth{};

		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissor{};

		// レンダーターゲットビュー設定
		for (auto [i, ref] : Indexed(subpass.colors)) {

			if (auto texture = attachments.at(ref.index).cast<TextureImpl>()) {
				auto& color = colors[i];

				color = texture->getRTV().getCpuHandle();

				m_cache.addTexture(*texture,D3D12_RESOURCE_STATE_RENDER_TARGET);

				viewport = texture->getViewport();
				scissor = texture->getScissorRect();

			} else {
				LOG_ERROR("無効なアタッチメントが設定されています。 [frameBuffer={},subpass={}]",m_frameBuffer->getName(),m_subpassIndex);
			}

		}

		// 深度ステンシルビュー設定
		for (auto [index, d] : Indexed(subpass.depth)) {

			if (auto texture = attachments.at(d.index).cast<TextureImpl>()) {

				depth = texture->getDSV().getCpuHandle();

				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_DEPTH_WRITE);

			} else {
				LOG_ERROR("無効なアタッチメントが設定されています。 [frameBuffer={},subpass={}]", m_frameBuffer->getName(), m_subpassIndex);
			}

		}

		// リソースバリア
		m_cache.recordCommand(*m_cmdList.Get());

		// レンダーターゲット設定
		m_cmdList->OMSetRenderTargets(
			(UINT)subpass.colors.size(),
			colors,
			FALSE,
			subpass.depth.empty() ? NULL : &depth
		);

		m_cmdList->RSSetViewports(1, &viewport);
		m_cmdList->RSSetScissorRects(1, &scissor);

		// 描画対象をクリア
		for (auto [index,attachmentDesc] : Indexed(attachmentDescs)) {

			if (auto texture = attachments[index].cast<TextureImpl>()) {

				if(attachmentDesc.clear == AttachmentClear::Clear) {

					texture->clear(m_cmdList.Get());

				}

			}

		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief      次のサブパスに進める
	//@―---------------------------------------------------------------------------
	void CommandListImpl::nextSubpass() {
		m_subpassIndex++;
		setSubpass();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーパス終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::endRenderPass() {
		
		if (!m_frameBuffer)return;

		auto& frameBufferDesc = m_frameBuffer->desc();
		auto& attachments = frameBufferDesc.attachments;
		auto& renderPassDesc = frameBufferDesc.renderPass->desc();

		for (auto [index,desc]: Indexed(renderPassDesc.attachments)) {

			auto& texture = attachments[index];
			if (auto pTexture = texture.cast<TextureImpl>()) {
				
				auto finalState = renderPassDesc.attachments[index].finalState;
				m_cache.addTexture(*pTexture, TypeConverter::Convert(finalState));

			}

			// TODO subresourceの扱いチェック
		}

		m_frameBuffer = nullptr;

	}


	//@―---------------------------------------------------------------------------
	//! @brief      ディスプレイにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void CommandListImpl::applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture)
	{
		if (auto pDisplay = display.cast<DisplayImpl>()) {
			pDisplay->recordApplyDisplay(*this, texture);
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  シザー矩形を設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setScissorRect(const IntRect* pRect, s32 num) {

		OB_ASSERT(num <= SCISSOR_RECT_MAX, "シザー矩形の最大数を超えました。[value={0},max={1}]", num, SCISSOR_RECT_MAX);
		D3D12_RECT rects[SCISSOR_RECT_MAX];
		for (s32 i = 0; i < num; ++i) {
			rects[i].left = pRect[i].left;
			rects[i].top = pRect[i].top;
			rects[i].right = pRect[i].right;
			rects[i].bottom = pRect[i].bottom;
		}
		m_cmdList->RSSetScissorRects(num, rects);

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ビューポートを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setViewport(const Viewport* pViewport, s32 num) {

		OB_ASSERT(num <= VIEWPORT_MAX, "ビューポートの最大数を超えました。[value={0},max={1}]", num, VIEWPORT_MAX);
		D3D12_VIEWPORT viewports[VIEWPORT_MAX];
		for (s32 i = 0; i < num; ++i) {
			viewports[i].TopLeftX = pViewport[i].left;
			viewports[i].TopLeftY = pViewport[i].top;
			viewports[i].Width = pViewport[i].width();
			viewports[i].Height = pViewport[i].height();
			viewports[i].MinDepth = pViewport[i].nearZ;
			viewports[i].MaxDepth = pViewport[i].farZ;
		}
		m_cmdList->RSSetViewports(num, viewports);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearColors(u32 mask) {

		OB_NOTIMPLEMENTED();
		//if (m_pRenderTarget == nullptr)return;
		//
		//auto& desc = m_pRenderTarget->getDesc();
		//for (s32 i = 0, ie = m_pRenderTarget->getColorTextureCount(); i < ie; ++i) {
		//	if (mask & (1 << i)) {
		//		if (m_hRTV[i].ptr) {
		//			auto texture = m_pRenderTarget->getColorTexture(i);
		//
		//			auto color = desc.colors[i].clearColor;
		//
		//			FLOAT values[4];
		//			values[0] = color.r;
		//			values[1] = color.g;
		//			values[2] = color.b;
		//			values[3] = color.a;
		//
		//			m_cmdList->ClearRenderTargetView(m_hRTV[i], values, 0, nullptr);
		//		}
		//	}
		//}

	}


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットのデプスとステンシルをクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearDepthStencil() {
		
		OB_NOTIMPLEMENTED();

		//auto& desc = m_pRenderTarget->getDesc();
		//if (desc.depth.empty())return;
		//if (m_hDSV.ptr != 0) {
		//	FLOAT depth = desc.depth[0].clearDepth;
		//	UINT8 stencil = (UINT8)desc.depth[0].clearDepth;
		//
		//	D3D12_CLEAR_FLAGS clearFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
		//	m_cmdList->ClearDepthStencilView(m_hDSV, clearFlags, depth, stencil, 0, nullptr);
		//}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      頂点バッファを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setVertexBuffers(Span<Ref<Buffer>> buffers) {
		StaticArray<D3D12_VERTEX_BUFFER_VIEW, VERTEX_BUFFER_MAX> views;
		if (views.size() <= buffers.size()) {
			LOG_ERROR("頂点バッファは{}以下である必要があります。[size={}]",views.size(),buffers.size());
			return;
		}

		size_t size = 0;
		for (auto& buffer : buffers) {
			if (buffer == nullptr) {
				LOG_ERROR("頂点バッファがnullです。");
				return;
			}
			
			if (auto pBuffer = buffer.cast<BufferImpl>()) {
				auto& view = views[size];
				view.BufferLocation = pBuffer->getNative()->GetGPUVirtualAddress();
				view.SizeInBytes = (UINT)pBuffer->getDesc().bufferSize;
				view.StrideInBytes = pBuffer->getDesc().bufferStride;
			} else {
				LOG_ERROR("空の頂点バッファが含まれています");
			}
			size++;
		}

		m_cmdList->IASetVertexBuffers(0, (UINT)size, views.data());
	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックスバッファを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setIndexBuffer(const Ref<Buffer>& buffer) {
		if (auto pBuffer = buffer.cast<BufferImpl>()) {
			D3D12_INDEX_BUFFER_VIEW view;
			view.BufferLocation = pBuffer->getNative()->GetGPUVirtualAddress();
			view.SizeInBytes = (UINT)pBuffer->getDesc().bufferSize;
			view.Format = pBuffer->getDesc().bufferStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
			m_cmdList->IASetIndexBuffer(&view);
		} else {

		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      パイプラインステートを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setPipelineState(const Ref<PipelineState>& pipeline) {
		if (auto p = pipeline.cast<PipelineStateImpl>()) {

			if (auto rootSignature = p->getRootSignature()) {

				if (m_rootSignature != rootSignature) {
					m_cmdList->SetGraphicsRootSignature(rootSignature);
					m_rootSignature = rootSignature;
				}
				m_cmdList->SetPipelineState(p->getNative());

				// TODO Geometryシェーダでのプリミティブ設定対応
				m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			}

		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void CommandListImpl::draw(const DrawParam& param) {
		m_cmdList->DrawInstanced(param.vertexCount, 1, param.startVertex,0);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックス描画
	//@―---------------------------------------------------------------------------
	void CommandListImpl::drawIndexed(const DrawIndexedParam& param) {
		m_cmdList->DrawIndexedInstanced(param.indexCount,1, param.startIndex,param.startVertex,0);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      デスクリプタテーブルを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setRootDesciptorTable(const rhi::SetDescriptorTableParam* params, s32 num) {
		for (s32 i = 0; i < num; ++i) {
			auto& param = params[i];
			if (auto pTable = param.table.cast<DescriptorTableImpl>()) {
				m_cmdList->SetGraphicsRootDescriptorTable(param.slot, pTable->getGpuHandle());
			} else {
				LOG_ERROR("空のDescriptorTableを含んでいます。");
			}
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  リソースバリアを挿入
	//@―---------------------------------------------------------------------------
	void CommandListImpl::insertResourceBarrier(const ResourceBarrier& resourceBarrier) {

		// ネイティブに変換
		// m_cmdList->ResourceBarrier(num, barriers.data());
	}

#pragma endregion

	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタハンドルのキャッシュをクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearDescriptorHandle() {
		m_hDSV.ptr = 0;
		for (s32 i = 0; i < std::size(m_hRTV); ++i) {
			m_hRTV[i].ptr = 0;
		}
	}



#ifdef OB_DEBUG
	void CommandListImpl::pushMarker(StringView name) {
		StringEncoder::Encode(name, m_markerNameCache);
		::PIXBeginEvent(m_cmdList.Get(),PIX_COLOR_DEFAULT, m_markerNameCache.data());
	}
	void CommandListImpl::popMarker() {
		::PIXEndEvent(m_cmdList.Get());
	}
#endif

}// namespace ob
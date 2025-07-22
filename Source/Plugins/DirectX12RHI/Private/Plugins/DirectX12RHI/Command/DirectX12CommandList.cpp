//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "DirectX12CommandList.h"
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Framework/RHI/RenderPass.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/SwapChain/DirectX12SwapChain.h>
#include <Plugins/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Plugins/DirectX12RHI/RootSignature/DirectX12RootSignature.h>
#include <Plugins/DirectX12RHI/PipelineState/DirectX12PipelineState.h>
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorTable.h>
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

#include <pix3.h>

//===============================================================
// # CommandListのマルチスレッド対応について
// * https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/recording-command-lists-and-bundles#recording-command-lists
// * https://shobomaru.wordpress.com/2015/04/20/d3d12-command/
// * https://shobomaru.wordpress.com/2015/07/12/d3d12-fence/
//===============================================================


namespace ob::rhi {

	//! @brief  コンストラクタ
	DirectX12CommandList::DirectX12CommandList(class DirectX12RHI& device, const CommandListDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		HRESULT result;
		auto type = TypeConverter::Convert(desc.type);

		// アロケータ生成
		result = device.getNative()->CreateCommandAllocator(type, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "ID3D12Device::CreateCommandAllocator()");
			return;
		}

		// コマンドリスト生成
		UINT nodeMask = 0;
		result = device.getNative()->CreateCommandList(nodeMask, type, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::OutputFatalLog(result, "ID3D12Device::CreateCommandList()");
			return;
		}

		// 初期状態で記録モードなのでクローズ
		m_cmdList->Close();

		Utility::SetName(m_cmdAllocator.Get(), getName());
		Utility::SetName(m_cmdList.Get(), getName());

		manage();
	}


	//! @brief  妥当な状態か
	bool DirectX12CommandList::isValid()const {
		return m_cmdList;
	}


	//! @brief      名前を取得
	const String& DirectX12CommandList::getName()const {
		return m_desc.name;
	}

#pragma endregion Command

	//! @brief  描画開始
	void DirectX12CommandList::begin() {
		HRESULT result;

		// コマンドアロケータをリセット
		result = m_cmdAllocator->Reset();
		if (FAILED(result)) {
			String message = "ID3D12CommandAllocator::Reset()\n";
			message += Utility::GetDebugLayerLastString(m_device.getNative().Get());
			Utility::OutputFatalLog(result, message);
		}

		// コマンドリストをリセット
		result = m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
		if (FAILED(result)) {
			String message = "ID3D12CommandList::Reset()\n";
			message += Utility::GetDebugLayerLastString(m_device.getNative().Get());
			Utility::OutputFatalLog(result, message);
		}

		Utility::SetName(m_cmdAllocator.Get(), getName());
		Utility::SetName(m_cmdList.Get(), getName());

		// デスクリプタヒープを設定
		m_device.setDescriptorHeaps(*this);

		m_signature = nullptr;
	}

	//! @brief  描画終了
	void DirectX12CommandList::end() {
		m_cmdList->Close();
	}

	//! @brief  コマンドをシステムキューに追加
	//! // TODO Singletonに依存しているので廃止
	void DirectX12CommandList::flush() {
		if (auto rhi = RHI::Get()) {
			Ref<CommandList> commandList = this;
			rhi->entryCommandList(commandList);
		}
	}

	//! @brief      RenderPass開始
	void DirectX12CommandList::beginRenderPass(const BeginPassParam& param) {

		clearRenderTargets();

		m_currentRenderPass = param;

		s32 width = 0;
		s32 height = 0;

		FixedVector<D3D12_RENDER_PASS_RENDER_TARGET_DESC, RENDER_TARGET_MAX> colors;
		D3D12_RENDER_PASS_DEPTH_STENCIL_DESC  depth;
		D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepth = nullptr;

		RenderTargetArray colorTextures;
		Ref<RenderTexture> depthTexture;

		m_cache.clear();

		for (auto [i, color] : Indexed(param.colors)) {
			auto& targetDesc = colors.emplace_back();
			if (auto texture = color.texture.cast<DirectX12Texture>()) {
				targetDesc.cpuDescriptor = texture->getRTV().getCpuHandle();
				targetDesc.BeginningAccess.Type = TypeConverter::Convert(color.beforeAccess);
				targetDesc.EndingAccess.Type = TypeConverter::Convert(color.afterAccess);

				width = texture->width();
				height = texture->height();

				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_RENDER_TARGET);
			}
		}
		{
			auto& targetDesc = depth;
			if (auto texture = param.depth.texture.cast<DirectX12Texture>()) {
				targetDesc.cpuDescriptor = texture->getDSV().getCpuHandle();
				targetDesc.DepthBeginningAccess.Type = TypeConverter::Convert(param.depth.beforeAccess);
				targetDesc.DepthEndingAccess.Type = TypeConverter::Convert(param.depth.afterAccess);

				width = texture->width();
				height = texture->height();

				pDepth = &depth;

				// TODO D3D12_RESOURCE_STATE_DEPTH_READが必要か確認する
				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			}
			if (auto texture = param.stencil.texture.cast<DirectX12Texture>()) {
				targetDesc.cpuDescriptor = texture->getRTV().getCpuHandle();
				targetDesc.StencilBeginningAccess.Type = TypeConverter::Convert(param.stencil.beforeAccess);
				targetDesc.StencilEndingAccess.Type = TypeConverter::Convert(param.stencil.afterAccess);

				width = texture->width();
				height = texture->height();

				pDepth = &depth;

				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			}

			if (param.depth.texture && param.stencil.texture && param.depth.texture != param.stencil.texture) {
				OB_ABORT("DepthとStencilが異なるテクスチャを指しています");
			}
		}

		D3D12_RENDER_PASS_FLAGS flags = D3D12_RENDER_PASS_FLAG_NONE;
		if (param.flags & RenderPassFlag::AllowUAVWrite) flags |= D3D12_RENDER_PASS_FLAG_ALLOW_UAV_WRITES;
		if (param.flags & RenderPassFlag::SuspendingPass) flags |= D3D12_RENDER_PASS_FLAG_SUSPENDING_PASS;
		if (param.flags & RenderPassFlag::ResumingPass) flags |= D3D12_RENDER_PASS_FLAG_RESUMING_PASS;


		// リソースバリア
		m_cache.recordCommand(*m_cmdList.Get());

		m_cmdList->BeginRenderPass(colors.size(), colors.data(), pDepth, flags);

		// 初期設定としてViewportとScissorRectを設定
		Viewport viewport(0, 0, width, height);
		IntRect scissor(0,0,width,height);

		setViewport(&viewport,1);
		setScissorRect(&scissor,1);

	}

	//! @brief RenderPass終了
	void DirectX12CommandList::endRenderPass() {

		m_cmdList->EndRenderPass();

		m_cache.clear();

		for (auto [i, color] : Indexed(m_colorTextures)) {
			if (auto texture = color.cast<DirectX12Texture>()) {
				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_COMMON);
			}
		}
		if (auto texture = m_depthTexture.cast<DirectX12Texture>()) {
			m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_COMMON);
		}

		// リソースバリア
		m_cache.recordCommand(*m_cmdList.Get());

		clearRenderTargets();

	}


	//! @brief      スワップチェーンにテクスチャを適用
	void DirectX12CommandList::applySwapChain(const Ref<SwapChain>& swapChain, const Ref<RenderTexture>& texture)
	{
		if (auto pSwapChain = swapChain.cast<DirectX12SwapChain>()) {
			pSwapChain->recordApplySwapChain(*this, texture);
		}
	}


	//! @brief  シザー矩形を設定
	void DirectX12CommandList::setScissorRect(const IntRect* pRect, s32 num) {

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


	//! @brief  ビューポートを設定
	void DirectX12CommandList::setViewport(const Viewport* pViewport, s32 num) {

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


	//! @brief      頂点バッファを設定
	void DirectX12CommandList::setVertexBuffers(Span<Ref<Buffer>> buffers, s32 first) {
		Array<D3D12_VERTEX_BUFFER_VIEW, VERTEX_BUFFER_MAX> views;
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
			
			if (auto pBuffer = buffer.cast<DirectX12Buffer>()) {
				// TODO SmallBufferAllocatorに対応してBufferLocationにオフセットを対応する
				auto& view = views[size];
				view.BufferLocation = pBuffer->getNative()->GetGPUVirtualAddress();
				view.SizeInBytes = (UINT)pBuffer->getDesc().size;
				view.StrideInBytes = pBuffer->getDesc().stride;
			} else {
				LOG_ERROR("空の頂点バッファが含まれています");
			}
			size++;
		}

		m_cmdList->IASetVertexBuffers(first, (UINT)size, views.data());
	}


	//! @brief      インデックスバッファを設定
	void DirectX12CommandList::setIndexBuffer(const Ref<Buffer>& buffer) {
		if (auto pBuffer = buffer.cast<DirectX12Buffer>()) {
			// TODO SmallBufferAllocatorに対応してBufferLocationにオフセットを対応する
			D3D12_INDEX_BUFFER_VIEW view;
			view.BufferLocation = pBuffer->getNative()->GetGPUVirtualAddress();
			view.SizeInBytes = (UINT)pBuffer->getDesc().size;
			view.Format = pBuffer->getDesc().stride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
			m_cmdList->IASetIndexBuffer(&view);
		} else {
			LOG_ERROR("空のインデックスバッファは指定できません");
		}
	}


	//! @brief      パイプラインステートを設定
	void DirectX12CommandList::setPipelineState(const Ref<PipelineState>& pipeline) {
		if (auto p = pipeline.cast<DirectX12PipelineState>()) {

			if (auto signature = p->getRootSignature()) {

				// Bindless時は特にルートシグネチャが共通しているので変更時のみ記録する
				if (m_signature != signature) {
					m_cmdList->SetGraphicsRootSignature(signature->getNative());
					m_signature = signature;
				}

				m_cmdList->SetPipelineState(p->getNative());

				// TODO Geometryシェーダでのプリミティブ設定対応
				m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			}

		}
	}


	//! @brief      描画
	void DirectX12CommandList::draw(const DrawParam& param) {
		m_cmdList->DrawInstanced(param.vertexCount, 1, param.startVertex,0);
	}


	//! @brief      インデックス描画
	void DirectX12CommandList::drawIndexed(const DrawIndexedParam& param) {
		m_cmdList->DrawIndexedInstanced(param.indexCount,1, param.startIndex,param.startVertex,0);
	}


	//! @brief      デスクリプタテーブルを設定
	void DirectX12CommandList::setRootDesciptorTable(const rhi::SetDescriptorTableParam* params, s32 num) {
		OB_ASSERT(m_signature!=nullptr,"先にPipelineStateを設定してください");

		for (s32 i = 0; i < num; ++i) {
			auto& param = params[i];

			if (!is_in_range(param.slot, m_signature->getDesc().layouts)) {
				LOG_ERROR("スロットが範囲外です");
				continue;
			}

			if (auto pTable = param.table.cast<DirectX12DescriptorTable>()) {
				pTable->record(*m_cmdList.Get(), *m_signature, param.slot);
			}
		}
	}


	//! @brief      ルート定数を設定
	void DirectX12CommandList::setRootConstant(const SetRootConstantsParam& param) {
		if (param.blob.size() % 4) {
			LOG_WARNING("ルート定数のサイズが4の倍数ではありません。");
		}
		if (param.blob.size()) {
			// DirectX12ではルート定数はスロット0に固定
			constexpr s32 slot = 0;
			m_cmdList->SetGraphicsRoot32BitConstants(slot, param.blob.size() / sizeof(s32), param.blob.data(), param.offset / sizeof(s32));
		}
	}


	//! @brief  GPUマーカーをプッシュ
	void DirectX12CommandList::pushMarker(StringView name) {
		StringEncoder::Encode(name, m_markerNameCache);
		::PIXBeginEvent(m_cmdList.Get(), PIX_COLOR_DEFAULT, m_markerNameCache.data());
	}


	//! @brief  GPUマーカーをポップ
	void DirectX12CommandList::popMarker() {
		::PIXEndEvent(m_cmdList.Get());
	}

#pragma endregion

	//! @brief  デスクリプタハンドルのキャッシュをクリア
	void DirectX12CommandList::clearRenderTargets() {
		m_colorTextures.clear();
		m_depthTexture = nullptr;
	}

}
//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DirectX12CommandList.h"
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Display/DirectX12Display.h>
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


namespace ob::rhi::dx12 {

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

		clearDescriptorHandle();

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

		m_rootSignature = nullptr;
	}

	//! @brief  描画終了
	void DirectX12CommandList::end() {
		m_cmdList->Close();
	}

	//! @brief  コマンドをシステムキューに追加
	//! // TODO Singletonに依存しているので廃止
	void DirectX12CommandList::flush() {
		if (auto rhi = RHI::Get()) {
			rhi->entryCommandList(*this);
		}
	}

	//! @brief      描画先設定
	void DirectX12CommandList::setRenderTargets(const RenderTextureArray& colors, const Ref<RenderTexture>& depth) {

		D3D12_CPU_DESCRIPTOR_HANDLE hColors[8]{};
		D3D12_CPU_DESCRIPTOR_HANDLE hDepth{};

		Ref<RenderTexture> tColors[8]{};
		Ref<RenderTexture> tDepth{};

		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissor{};

		m_cache.clear();


		for (auto [i, color] : Indexed(m_colorTextures)) {

			bool has = false;
			for (auto& c : colors) {
				if (c == color) {
					has = true;
				}
			}

			if (!has) {
				if (auto texture = color.cast<DirectX12Texture>()) {
					m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_COMMON);
				}
			}
			m_colorTextures[i] = {};
			hColors[i] = {};
		}
		if (auto texture = m_depthTexture.cast<DirectX12Texture>()) {

			if (m_depthTexture != depth) {
				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_COMMON);
			}
			m_depthTexture = {};
			m_hDSV = {};
		}

		// レンダーターゲットビュー設定
		for (auto [i, color] : Indexed(colors)) {

			if (auto texture = color.cast<DirectX12Texture>()) {
				
				m_cache.addTexture(*texture,D3D12_RESOURCE_STATE_RENDER_TARGET);
				hColors[i] = texture->getRTV().getCpuHandle();
				tColors[i] = texture;

				viewport = texture->getViewport();
				scissor = texture->getScissorRect();

			} else {
				LOG_ERROR("無効なレンダーターゲットが設定されています。");
				return;
			}

		}

		// 深度ステンシルビュー設定
		{

			if (auto texture = depth.cast<DirectX12Texture>()) {

				m_cache.addTexture(*texture, D3D12_RESOURCE_STATE_DEPTH_WRITE);
				hDepth = texture->getDSV().getCpuHandle();
				tDepth = texture;
			}

		}

		// リソースバリア
		m_cache.recordCommand(*m_cmdList.Get());

		// レンダーターゲット設定
		m_cmdList->OMSetRenderTargets(
			colors.size(),
			hColors,
			FALSE,
			depth ? &hDepth : NULL
		);

		m_cmdList->RSSetViewports(1, &viewport);
		m_cmdList->RSSetScissorRects(1, &scissor);

		for (auto [i, item] : Indexed(hColors))m_hRTVs[i] = item;
		m_hDSV = hDepth;
		for (auto [i, item] : Indexed(tColors))m_colorTextures[i] = item;
		m_depthTexture = tDepth;

	}
	

	//! @brief      ディスプレイにテクスチャを適用
	void DirectX12CommandList::applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture)
	{
		if (auto pDisplay = display.cast<DirectX12Display>()) {
			pDisplay->recordApplyDisplay(*this, texture);
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


	//! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
	void DirectX12CommandList::clearColors(u32 mask) {

		for (auto [i, handle] : Indexed(m_hRTVs)) {
			if (!(mask & (1 << i)))continue;
			if (!handle.ptr) continue;
			if (!m_colorTextures[i]) continue;
			
			auto color = m_colorTextures[i]->descOfRenderTexture().clear.color;

			FLOAT values[4];
			values[0] = color.r;
			values[1] = color.g;
			values[2] = color.b;
			values[3] = color.a;
			m_cmdList->ClearRenderTargetView(handle, values, 0, nullptr);
		}

	}


	//! @brief      レンダーターゲットのデプスとステンシルをクリア
	void DirectX12CommandList::clearDepthStencil() {
		
		if (m_hDSV.ptr != 0 && m_depthTexture) {
			auto& desc = m_depthTexture->descOfRenderTexture();
			FLOAT depth = desc.clear.depth;
			UINT8 stencil = desc.clear.stencil;
		
			D3D12_CLEAR_FLAGS clearFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
			m_cmdList->ClearDepthStencilView(m_hDSV, clearFlags, depth, stencil, 0, nullptr);
		}
	}


	//! @brief      頂点バッファを設定
	void DirectX12CommandList::setVertexBuffers(Span<Ref<Buffer>> buffers) {
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

		m_cmdList->IASetVertexBuffers(0, (UINT)size, views.data());
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

			if (auto rootSignature = p->getRootSignature()) {

				// Bindless時は特にルートシグネチャが共通しているので変更時のみ記録する
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
		for (s32 i = 0; i < num; ++i) {
			auto& param = params[i];
			if (auto pTable = param.table.cast<DirectX12DescriptorTable>()) {
				pTable->record(*m_cmdList.Get(),param.slot);
			}
		}
	}


	//! @brief      ルート定数を設定
	void DirectX12CommandList::setRootConstant(const SetRootConstantsParam& param) {
		if (param.blob.size() % 4) {
			LOG_WARNING("ルート定数のサイズが4の倍数ではありません。");
		}
		if (param.blob.size()) {
			m_cmdList->SetGraphicsRoot32BitConstants(param.slot, param.blob.size() / sizeof(s32), param.blob.data(), param.offset / sizeof(s32));
		}
	}


	//! @brief  リソースバリアを挿入
	void DirectX12CommandList::insertResourceBarrier(const ResourceBarrier& resourceBarrier) {

		// ネイティブに変換
		// m_cmdList->ResourceBarrier(num, barriers.data());
	}

#pragma endregion

	//! @brief  デスクリプタハンドルのキャッシュをクリア
	void DirectX12CommandList::clearDescriptorHandle() {
		m_hDSV.ptr = 0;
		for (s32 i = 0; i < std::size(m_hRTVs); ++i) {
			m_hRTVs[i].ptr = 0;
		}
	}

	//! @brief  GPUマーカーをプッシュ
	void DirectX12CommandList::pushMarker(StringView name) {
		StringEncoder::Encode(name, m_markerNameCache);
		::PIXBeginEvent(m_cmdList.Get(),PIX_COLOR_DEFAULT, m_markerNameCache.data());
	}

	//! @brief  GPUマーカーをポップ
	void DirectX12CommandList::popMarker() {
		::PIXEndEvent(m_cmdList.Get());
	}

}
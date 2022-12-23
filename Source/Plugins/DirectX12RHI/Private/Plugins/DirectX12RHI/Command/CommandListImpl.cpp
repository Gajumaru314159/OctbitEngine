//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "CommandListImpl.h"
#include <Framework/RHI/Device.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/RenderTarget.h>
#include <Framework/RHI/Types/CommandParam.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Display/DisplayImpl.h>
#include <Plugins/DirectX12RHI/Texture/RenderTargetImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Plugins/DirectX12RHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/DirectX12RHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorTableImpl.h>
#include <Plugins/DirectX12RHI/RenderPass/RenderPassImpl.h>
#include <Plugins/DirectX12RHI/FrameBuffer/FrameBufferImpl.h>
#include <Plugins/DirectX12RHI/Texture/RenderTargetImpl.h>
#include <Plugins/DirectX12RHI/Texture/RenderTextureImpl.h>
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

	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool CommandListImpl::isValid()const {
		return m_cmdList;
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

		// デスクリプタヒープを設定
		m_device.setDescriptorHeaps(*this);

	}


	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::end() {
		m_cmdList->Close();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      レンダーパス開始
	//@―---------------------------------------------------------------------------
	void CommandListImpl::beginRenderPass(const FrameBuffer& frameBuffer) {

		m_frameBuffer = frameBuffer;
		m_subpassIndex = 0;

		setSubpass();

	}

	//@―---------------------------------------------------------------------------
	//! @brief      サブパス設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setSubpass() {
		
		if (!m_frameBuffer)return;

		auto& frameBufferDesc = m_frameBuffer.desc();
		auto& attachments = frameBufferDesc.attachments;
		auto& renderPassDesc = frameBufferDesc.renderPass.desc();

		if (!is_in_range(m_subpassIndex, renderPassDesc.subpasses)){
			LOG_WARNING("サブパスが範囲外です。[index={},size={}]",m_subpassIndex, renderPassDesc.subpasses.size());
			return;
		}

		auto& subpass= renderPassDesc.subpasses[m_subpassIndex];

		D3D12_CPU_DESCRIPTOR_HANDLE colors[8];
		D3D12_CPU_DESCRIPTOR_HANDLE depth;

		// レンダーターゲットビュー設定
		for (auto [i, ref] : Indexed(subpass.colors)) {

			auto& rRenderTexture = Device::GetImpl<RenderTextureImpl>(attachments.at(ref.index));
			auto& color = colors[i];

			color = rRenderTexture.getRTV().getCpuHandle();

			m_cache.addTexture(rRenderTexture.getResource().Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET,0);

		}

		// 深度ステンシルビュー設定
		for (auto [index, d] : Indexed(subpass.depth)) {

			auto& rRenderTexture = Device::GetImpl<RenderTextureImpl>(attachments.at(d.index));
			
			depth = rRenderTexture.getDSV().getCpuHandle();

			m_cache.addTexture(rRenderTexture.getResource().Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE, 0);

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

		//auto viewport = rTarget.getViewport();
		//auto rect = rTarget.getScissorRect();
		//m_cmdList->RSSetViewports(1, &viewport);
		//m_cmdList->RSSetScissorRects(1, &rect);
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

		auto& frameBufferDesc = m_frameBuffer.desc();
		auto& attachments = frameBufferDesc.attachments;
		auto& renderPassDesc = frameBufferDesc.renderPass.desc();

		for (auto [index,desc]: Indexed(renderPassDesc.attachments)) {

			auto& texture = attachments[index];

			// attachment.finalStateへのステート変更
			// subresourceの扱い要チェック
		}

		m_frameBuffer.release();

	}


	//@―---------------------------------------------------------------------------
	//! @brief      スワップチェーンにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void CommandListImpl::applyDisplay(const Display& display, const Texture& texture)
	{
		auto& rDisplay = Device::GetImpl<DisplayImpl>(display);
		auto& rTexture = Device::GetImpl<TextureImpl>(texture);

		{
			D3D12_RESOURCE_BARRIER barrier[2] = {
				CD3DX12_RESOURCE_BARRIER::Transition(rDisplay.getResource(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST),
				CD3DX12_RESOURCE_BARRIER::Transition(rTexture.getResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE),
			};
			m_cmdList->ResourceBarrier((UINT)std::size(barrier), barrier);
		}

		{
			auto desc = rTexture.getResource()->GetDesc();
			auto desc2 = rDisplay.getResource()->GetDesc();

			const int maxSubresources = 100;
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprints[maxSubresources];
			UINT numRows[maxSubresources];
			UINT64 rowSizeInBytes[maxSubresources], uploadSize;
			
			m_device.getNative()->GetCopyableFootprints(&desc,0,1,0, footprints, numRows, rowSizeInBytes, &uploadSize);

			m_cmdList->CopyTextureRegion(
				&CD3DX12_TEXTURE_COPY_LOCATION(rDisplay.getResource(), 0),
				0, 0, 0,
				&CD3DX12_TEXTURE_COPY_LOCATION(rTexture.getResource(),0),
				nullptr);
		}


		{
			D3D12_RESOURCE_BARRIER barrier[2] = {
				CD3DX12_RESOURCE_BARRIER::Transition(rDisplay.getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT),
				CD3DX12_RESOURCE_BARRIER::Transition(rTexture.getResource(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
			};
			m_cmdList->ResourceBarrier((UINT)std::size(barrier), barrier);
		}
	}

	/*
	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::beginRender(const RenderTarget& target) {
		auto count = target.getColorTextureCount();
		auto& rTarget = Device::GetImpl<RenderTargetImpl>(target);

		clearDescriptorHandle();
		m_pRenderTarget = &target;


		// RTV更新
		for (s32 i = 0; i < count; ++i) {
			m_hRTV[i] = rTarget.getColorCpuHandle(i);
		}

		// DSV更新
		if (target.hasDepth()) {
			m_hDSV = rTarget.getDepthCpuHandle();
		}

		// コマンド発行
		const D3D12_CPU_DESCRIPTOR_HANDLE* pRTV = nullptr;
		const D3D12_CPU_DESCRIPTOR_HANDLE* pDSV = nullptr;
		if (count)pRTV = m_hRTV;
		if (target.hasDepth())pDSV = &m_hDSV;

		m_cmdList->OMSetRenderTargets(count, pRTV, false, pDSV);

		auto viewport= rTarget.getViewport();
		auto rect = rTarget.getScissorRect();
		m_cmdList->RSSetViewports(1, &viewport);
		m_cmdList->RSSetScissorRects(1, &rect);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::endRender() {
		m_pRenderTarget = nullptr;
	}
	*/

	//@―---------------------------------------------------------------------------
	//! @brief  シザー矩形を設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setScissorRect(const IntRect* pRect, s32 num) {

		OB_CHECK_ASSERT(num <= SCISSOR_RECT_MAX, "シザー矩形の最大数を超えました。[value={0},max={1}]", num, SCISSOR_RECT_MAX);
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

		OB_CHECK_ASSERT(num <= VIEWPORT_MAX, "ビューポートの最大数を超えました。[value={0},max={1}]", num, VIEWPORT_MAX);
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
	void CommandListImpl::setVertexBuffers(Span<const Buffer*> buffers) {
		StaticArray<D3D12_VERTEX_BUFFER_VIEW,16> views;
		if (views.size() <= buffers.size()) {
			LOG_ERROR("頂点バッファは{}以下である必要があります。[size={}]",views.size(),buffers.size());
			return;
		}

		size_t size = 0;
		for (auto buffer : buffers) {
			if (buffer == nullptr) {
				LOG_ERROR("頂点バッファがnullです。");
				return;
			}

			auto& rBuffer = Device::GetImpl<BufferImpl>(*buffer);
			auto& view=views[size];
			view.BufferLocation = rBuffer.getNative()->GetGPUVirtualAddress();
			view.SizeInBytes = (UINT)rBuffer.getDesc().bufferSize;
			view.StrideInBytes = rBuffer.getDesc().bufferStride;
			size++;
		}

		m_cmdList->IASetVertexBuffers(0, (UINT)size, views.data());
	}


	//@―---------------------------------------------------------------------------
	//! @brief      インデックスバッファを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setIndexBuffer(const Buffer& buffer) {
		auto& rBuffer = Device::GetImpl<BufferImpl>(buffer);
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = rBuffer.getNative()->GetGPUVirtualAddress();
		view.SizeInBytes = (UINT)rBuffer.getDesc().bufferSize;
		view.Format = rBuffer.getDesc().bufferStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		m_cmdList->IASetIndexBuffer(&view);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ルートシグネチャを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setRootSignature(const RootSignature& signature) {
		auto& rSignature = Device::GetImpl<RootSignatureImpl>(signature);
		m_cmdList->SetGraphicsRootSignature(rSignature.getNative());
	}


	//@―---------------------------------------------------------------------------
	//! @brief      パイプラインステートを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setPipelineState(const PipelineState& pipeline) {
		auto& rPipeline = Device::GetImpl<PipelineStateImpl>(pipeline);
		m_cmdList->SetPipelineState(rPipeline.getNative());
		//auto topology = TypeConverter::Convert(pPipeline->getDesc().topology);
		// TODO IASetPrimitiveTopology
		m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
			auto& rTable = Device::GetImpl<DescriptorTableImpl>(param.table);
			m_cmdList->SetGraphicsRootDescriptorTable(param.slot,rTable.getGpuHandle());
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
	//! @brief  名前変更時
	//@―---------------------------------------------------------------------------
	void CommandListImpl::onNameChanged(){
		Utility::setName(m_cmdAllocator.Get(), getName());
		Utility::setName(m_cmdList.Get(), getName());
	}

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
	void CommandListImpl::pushMarker(const char* pName) {
		//::PIXBeginEvent(m_cmdList.Get(),PIX_COLOR_DEFAULT, pName);
	}
	void CommandListImpl::popMarker() {
		//::PIXEndEvent();
	}
#endif

}// namespace ob
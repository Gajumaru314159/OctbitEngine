//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "CommandListImpl.h"
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Constants.h>
#include <Framework/Graphic/RenderTarget.h>
#include <Framework/Graphic/Types/CommandParam.h>
#include <Framework/Graphic/Buffer.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/SwapChain/SwapChainImpl.h>
#include <Plugins/GraphicDirectX12/Texture/RenderTargetImpl.h>
#include <Plugins/GraphicDirectX12/Texture/TextureImpl.h>
#include <Plugins/GraphicDirectX12/RootSignature/RootSignatureImpl.h>
#include <Plugins/GraphicDirectX12/PipelineState/PipelineStateImpl.h>
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorTableImpl.h>
#include <Plugins/GraphicDirectX12/Texture/RenderTargetImpl.h>
#include <Plugins/GraphicDirectX12/Buffer/BufferImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>
#include <pix3.h>

//===============================================================
// # CommandListのマルチスレッド対応について
// * https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/recording-command-lists-and-bundles#recording-command-lists
// * https://shobomaru.wordpress.com/2015/04/20/d3d12-command/
// * https://shobomaru.wordpress.com/2015/07/12/d3d12-fence/
//===============================================================


namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandListImpl::CommandListImpl(class DeviceImpl& device, const CommandListDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		HRESULT result;
		auto type = TypeConverter::convert(desc.type);

		// アロケータ生成
		result = device.getNative()->CreateCommandAllocator(type, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommandAllocator()"));
			return;
		}

		// コマンドリスト生成
		UINT nodeMask = 0;
		result = device.getNative()->CreateCommandList(nodeMask, type, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommandList()"));
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

		m_pRenderTarget = nullptr;
		clearDescriptorHandle();

		// コマンドアロケータをリセット
		result = m_cmdAllocator->Reset();
		if (FAILED(result)) {
			String message = TC("ID3D12CommandAllocator::Reset()\n");
			message += Utility::getDebugLayerLastString(m_device.getNative().Get());
			Utility::outputFatalLog(result, message);
		}

		// コマンドリストをリセット
		result = m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
		if (FAILED(result)) {
			String message = TC("ID3D12CommandList::Reset()\n");
			message += Utility::getDebugLayerLastString(m_device.getNative().Get());
			Utility::outputFatalLog(result, message);
		}

		// デスクリプタヒープを設定
		m_device.setDescriptorHeaps(*this);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  描画終了
	//@―---------------------------------------------------------------------------
	void CommandListImpl::end() {
		// リソースバリア設定
		m_cmdList->Close();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      スワップチェーンにテクスチャを適用
	//@―---------------------------------------------------------------------------
	void CommandListImpl::applySwapChain(const SwapChain& swapChain, const Texture& texture)
	{
		auto& rSwapChain = Device::GetImpl<SwapChainImpl>(swapChain);
		auto& rTexture = Device::GetImpl<TextureImpl>(texture);

		{
			D3D12_RESOURCE_BARRIER barrier[2] = {
				CD3DX12_RESOURCE_BARRIER::Transition(rSwapChain.getResource(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST),
				CD3DX12_RESOURCE_BARRIER::Transition(rTexture.getResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE),
			};
			m_cmdList->ResourceBarrier(get_size(barrier), barrier);
		}

		{
			auto desc = rTexture.getResource()->GetDesc();
			auto desc2 = rSwapChain.getResource()->GetDesc();

			const int maxSubresources = 100;
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprints[maxSubresources];
			UINT numRows[maxSubresources];
			UINT64 rowSizeInBytes[maxSubresources], uploadSize;
			
			m_device.getNative()->GetCopyableFootprints(&desc,0,1,0, footprints, numRows, rowSizeInBytes, &uploadSize);

			m_cmdList->CopyTextureRegion(
				&CD3DX12_TEXTURE_COPY_LOCATION(rSwapChain.getResource(), 0),
				0, 0, 0,
				&CD3DX12_TEXTURE_COPY_LOCATION(rTexture.getResource(),0),
				nullptr);
		}


		{
			D3D12_RESOURCE_BARRIER barrier[2] = {
				CD3DX12_RESOURCE_BARRIER::Transition(rSwapChain.getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT),
				CD3DX12_RESOURCE_BARRIER::Transition(rTexture.getResource(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
			};
			m_cmdList->ResourceBarrier(get_size(barrier), barrier);
		}
	}


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
		if (m_pRenderTarget == nullptr)return;

		auto& desc = m_pRenderTarget->getDesc();
		for (s32 i = 0, ie = m_pRenderTarget->getColorTextureCount(); i < ie; ++i) {
			if (mask & (1 << i)) {
				if (m_hRTV[i].ptr) {
					auto texture = m_pRenderTarget->getColorTexture(i);

					auto color = desc.colors[i].clearColor;

					FLOAT values[4];
					values[0] = color.r;
					values[1] = color.g;
					values[2] = color.b;
					values[3] = color.a;

					m_cmdList->ClearRenderTargetView(m_hRTV[i], values, 0, nullptr);
				}
			}
		}

	}


	//@―---------------------------------------------------------------------------
	//! @brief      レンダーターゲットのデプスとステンシルをクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearDepthStencil() {
		auto& desc = m_pRenderTarget->getDesc();
		if (desc.depth.empty())return;
		if (m_hDSV.ptr != 0) {
			FLOAT depth = desc.depth[0].clearDepth;
			UINT8 stencil = (UINT8)desc.depth[0].clearDepth;

			D3D12_CLEAR_FLAGS clearFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
			m_cmdList->ClearDepthStencilView(m_hDSV, clearFlags, depth, stencil, 0, nullptr);
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      頂点バッファを設定
	//@―---------------------------------------------------------------------------
	void CommandListImpl::setVertexBuffer(const Buffer& buffer) {
		auto& rBuffer = Device::GetImpl<BufferImpl>(buffer);
		D3D12_VERTEX_BUFFER_VIEW view;
		view.BufferLocation = rBuffer.getNative()->GetGPUVirtualAddress();
		view.SizeInBytes = (UINT)rBuffer.getDesc().bufferSize;
		view.StrideInBytes = rBuffer.getDesc().bufferStride;
		m_cmdList->IASetVertexBuffers(0, 1, &view);
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
		//auto topology = TypeConverter::convert(pPipeline->getDesc().topology);
		// TODO IASetPrimitiveTopology
		m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void CommandListImpl::draw(const DrawParam& param) {
		m_cmdList->DrawInstanced(param.indexCount, 1, param.startVertex,0);
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
	void CommandListImpl::setRootDesciptorTable(const graphic::SetDescriptorTableParam* params, s32 num) {
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
	//! @brief  デスクリプタハンドルのキャッシュをクリア
	//@―---------------------------------------------------------------------------
	void CommandListImpl::clearDescriptorHandle() {
		m_hDSV.ptr = 0;
		for (s32 i = 0; i < get_size(m_hRTV); ++i) {
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
//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "RenderTextureImpl.h"
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>

#include <Framework/RHI/Device.h>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderTextureImpl::RenderTextureImpl(DeviceImpl& rDevice, const RenderTextureDesc& desc)
		: m_desc(desc)
	{
		auto format = TypeConverter::Convert(desc.format);
		D3D12_CLEAR_VALUE* pClearValue = nullptr;

		const FLOAT clearColor[4] = { desc.clear.color.r,desc.clear.color.g,desc.clear.color.b,desc.clear.color.a };
		auto colorClearValue = CD3DX12_CLEAR_VALUE(format, clearColor);
		auto depthClearValue = CD3DX12_CLEAR_VALUE(format, 1.0f, 0);

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc{};
		D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;

		if (TextureFormatUtility::HasDepth(desc.format) == false) {

			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			resourceStates |= D3D12_RESOURCE_STATE_RENDER_TARGET;
			pClearValue = &colorClearValue;

		} else {

			OB_CHECK_ASSERT(TextureFormatUtility::HasDepth(desc.format), "デプス・ステンシルに非対応なフォーマットです。");
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			resourceStates |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
			pClearValue = &depthClearValue;

		}

		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


		ComPtr<ID3D12Resource> resource;

		auto result = rDevice.getNative()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			resourceStates,
			pClearValue,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
		}

#ifdef OB_DEBUG
		{
			resource->SetName(TEXT("UnnamedRenderTexture"));
		}
#endif

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);
		rDevice.allocateHandle(DescriptorHeapType::RTV, m_hRTV, 1);

		m_resource = resource;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	RenderTextureImpl::~RenderTextureImpl() {
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool RenderTextureImpl::isValid()const {
		return m_resource && !m_hSRV.empty() && !m_hRTV.empty();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  サイズ
	//@―---------------------------------------------------------------------------
	Size RenderTextureImpl::size()const {
		return m_desc.size;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ・フォーマットを取得
	//@―---------------------------------------------------------------------------
	TextureFormat RenderTextureImpl::format()const {
		return m_desc.format;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ミップレベルを取得
	//@―---------------------------------------------------------------------------
	s32 RenderTextureImpl::mipLevels()const {
		return 1;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      デプスのデスクリプタCPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE RenderTextureImpl::getCPUHandle()const {
		return m_hSRV.getCpuHandle();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      デプスのデスクリプタGPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE RenderTextureImpl::getGPUHandle()const {
		return m_hSRV.getGpuHandle();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ビューポートを取得
	//@―---------------------------------------------------------------------------
	D3D12_VIEWPORT RenderTextureImpl::getViewport()const {
		return m_viewport;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      シザー矩形を取得
	//@―---------------------------------------------------------------------------
	D3D12_RECT RenderTextureImpl::getScissorRect()const {
		return m_scissorRect;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  名前変更時
	//@―---------------------------------------------------------------------------
	void RenderTextureImpl::onNameChanged() {
		Utility::setName(m_resource.Get(), getName());
	}

}// namespace ob::rhi::dx12
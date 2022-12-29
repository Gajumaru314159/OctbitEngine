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
		: TextureImpl(rDevice)
		, m_desc(desc)
	{
		// クリアカラー設定
		const FLOAT clearColor[4] = { desc.clear.color.r,desc.clear.color.g,desc.clear.color.b,desc.clear.color.a };
		auto format = TypeConverter::Convert(desc.format);
		auto colorClearValue = CD3DX12_CLEAR_VALUE(format, clearColor);
		auto depthClearValue = CD3DX12_CLEAR_VALUE(format, 1.0f, 0);

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
		D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;

		D3D12_CLEAR_VALUE* pClearValue = nullptr;
		const bool isColor = !TextureFormatUtility::HasDepth(desc.format);
		const bool isDepth = !isColor;

		if (isColor) {

			OB_ASSERT(!TextureFormatUtility::HasDepth(desc.format), "カラーに非対応なフォーマットです。");
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			resourceStates = D3D12_RESOURCE_STATE_RENDER_TARGET;
			pClearValue = &colorClearValue;

		} 
		if(isDepth){

			OB_ASSERT(TextureFormatUtility::HasDepth(desc.format), "デプス・ステンシルに非対応なフォーマットです。");
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			resourceStates = D3D12_RESOURCE_STATE_DEPTH_WRITE;
			pClearValue = &depthClearValue;

		}

		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


		// いソース生成

		ComPtr<ID3D12Resource> resource;

		auto result = rDevice.getNative()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			resourceStates,
			pClearValue,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
		}

		// View生成
		if (isColor) {
			D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			viewDesc.Format = resource->GetDesc().Format;
			viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			viewDesc.Texture2D.MipLevels = 1;

			rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hSRV.getCpuHandle();
			rDevice.getNative()->CreateShaderResourceView(resource.Get(), &viewDesc, handle);
		}

		if (isColor) {

			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Format = format;

			rDevice.allocateHandle(DescriptorHeapType::RTV, m_hRTV, 1);

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hRTV.getCpuHandle();
			rDevice.getNative()->CreateRenderTargetView(resource.Get(), &viewDesc, handle);

		} 
		if(isDepth){

			D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
			viewDesc.Format = format;
			viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			viewDesc.Flags = D3D12_DSV_FLAG_NONE;

			rDevice.allocateHandle(DescriptorHeapType::DSV, m_hDSV, 1);

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hDSV.getCpuHandle();
			rDevice.getNative()->CreateDepthStencilView(resource.Get(), &viewDesc, handle);

		}

		m_viewport = CD3DX12_VIEWPORT(resource.Get());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		m_resource = resource;

		{
			TextureDesc baseDesc;
			//TextureImpl::m_desc.
		}

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
		return m_resource;
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
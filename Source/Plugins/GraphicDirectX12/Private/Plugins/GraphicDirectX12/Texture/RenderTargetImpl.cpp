//***********************************************************
//! @file
//! @brief		レンダーターゲット実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "RenderTargetImpl.h"
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Texture/TextureImpl.h>

#include <Framework/Graphic/Device.h>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderTargetImpl::RenderTargetImpl(DeviceImpl& rDevice, const RenderTargetDesc& desc)
		: m_desc(desc)
	{

		const s32 colorCount = std::size(m_desc.colors);
		const s32 depthCount = m_desc.depth.empty() ? 0 : 1;
		auto& nativeDevice = rDevice.getNative();
		OB_CHECK_ASSERT(0 < colorCount && colorCount <= 8, "マルチターゲットの数が不正です。1以上8以下にしてください。[{}]", colorCount);

		// ハンドルを生成
		if(colorCount)rDevice.allocateHandle(graphic::DescriptorHeapType::RTV, m_hRTV, colorCount);
		if(depthCount)rDevice.allocateHandle(graphic::DescriptorHeapType::DSV, m_hDSV, depthCount);

		// レンダーターゲットを生成
		s32 index = 0;
		for (auto& element : m_desc.colors) {

			auto format = TypeConverter::Convert(element.format);

			TextureDesc tdesc;
			tdesc.size = m_desc.size;
			tdesc.type = TextureType::RenderTarget;
			tdesc.format = element.format;
			tdesc.arrayNum = 0;
			tdesc.mipLevel = 1;
			tdesc.color = element.clearColor;

			// メインリソースを生成
			auto& texture = m_textures.emplace_back(Texture(tdesc));
			auto& rTexture = Device::GetImpl<TextureImpl>(texture);

			// レンダーターゲットビューを生成
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Format = format;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hRTV.getCpuHandle(index);
			nativeDevice->CreateRenderTargetView(rTexture.getResource(), &rtvDesc, handle);

			++index;

		}

		// デプス・ステンシル生成
		if (0 < depthCount) {

			auto& element = m_desc.depth[0];
			auto format = TypeConverter::Convert(element.format);

			TextureDesc tdesc;
			tdesc.size = m_desc.size;
			tdesc.type = TextureType::DepthStencil;
			tdesc.format = element.format;
			tdesc.arrayNum = 0;
			tdesc.mipLevel = 1;

			// メインリソースを生成
			m_depth = Texture(tdesc);
			auto& rTexture = Device::GetImpl<TextureImpl>(m_depth);

			//深度ビュー作成
			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = rTexture.getResource()->GetDesc().Format;//デプス値に32bit使用
			dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hDSV.getCpuHandle();
			nativeDevice->CreateDepthStencilView(rTexture.getResource(), &dsvDesc, handle);

		}

		// 0番目のターゲットをビューポートサイズとする
		m_viewport = CD3DX12_VIEWPORT(Device::GetImpl<TextureImpl>(m_textures[0]).getResource());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		m_initialized = true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	RenderTargetImpl::~RenderTargetImpl() {
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool RenderTargetImpl::isValid()const {
		return m_initialized;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      定義を取得
	//@―---------------------------------------------------------------------------
	const RenderTargetDesc& RenderTargetImpl::getDesc()const {
		return m_desc;
	}


	//@―---------------------------------------------------------------------------
	//! @brief          テクスチャを取得
	//! 
	//! @param index    マルチレンダーターゲットのインデックス
	//@―---------------------------------------------------------------------------
	Texture RenderTargetImpl::getColorTexture(s32 index)const {
		if (is_in_range(index, m_textures)) {
			return m_textures[index];
		}
		return Texture();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デプス・テクスチャを取得
	//@―---------------------------------------------------------------------------
	Texture RenderTargetImpl::getDepthTexture()const {
		return m_depth;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画ターゲット枚数を取得
	//! 
	//! @details    デプス・ステンシルテクスチャは含まれません。
	//@―---------------------------------------------------------------------------
	s32 RenderTargetImpl::getColorTextureCount()const {
		return gsl::narrow_cast<s32>(m_desc.colors.size());
	}


	//@―---------------------------------------------------------------------------
	//! @brief      カラーのデスクリプタCPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetImpl::getColorCpuHandle(s32 index)const {
		return m_hRTV.getCpuHandle(index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      カラーのデスクリプタGPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE RenderTargetImpl::getColorGpuHandle(s32 index)const {
		return m_hRTV.getGpuHandle(index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      デプスのデスクリプタCPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetImpl::getDepthCpuHandle()const {
		return m_hDSV.getCpuHandle();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      デプスのデスクリプタGPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE RenderTargetImpl::getDepthGpuHandle()const {
		return m_hDSV.getGpuHandle();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ビューポートを取得
	//@―---------------------------------------------------------------------------
	D3D12_VIEWPORT RenderTargetImpl::getViewport()const {
		return m_viewport;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      シザー矩形を取得
	//@―---------------------------------------------------------------------------
	D3D12_RECT RenderTargetImpl::getScissorRect()const {
		return m_scissorRect;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  名前変更時
	//@―---------------------------------------------------------------------------
	void RenderTargetImpl::onNameChanged() {
		s32 index = 0;
		for (auto& texture : m_textures) {
			String childName = Format(TC("{}_Color{}"), getName(), index);
			texture.setName(childName);
			index++;
		}
		{
			String childName = Format(TC("{}_Depth"), getName());
			m_depth.setName(childName);
		}
	}

}// namespace ob::graphic::dx12
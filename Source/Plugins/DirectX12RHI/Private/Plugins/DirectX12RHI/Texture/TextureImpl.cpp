//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "TextureImpl.h"
#include <Framework/RHI/Display.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/Command/ResourceStateCache.h>
#include <DirectXTex.h>


namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      TextureDesc から空のテクスチャを生成
    //@―---------------------------------------------------------------------------
    TextureImpl::TextureImpl(DirectX12RHI& rDevice, const TextureDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{

		if (m_desc.size.width <= 0 || m_desc.size.height <= 0) {
			LOG_ERROR("Textureの生成に失敗。サイズが不正です。[size=({},{})]", m_desc.size.width, m_desc.size.height);
			return;
		}

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		auto format = TypeConverter::Convert(desc.format);
		
		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.DepthOrArraySize = std::max(desc.arrayNum, 1);
		resourceDesc.MipLevels = std::max(desc.mipLevels, 0);
		
		switch (desc.type) {
		case TextureType::Texture1D:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex1D(format, desc.size.width);
			break;

		case TextureType::Texture2D:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			break;

		case TextureType::Texture3D:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex3D(format, desc.size.width, desc.size.height,desc.size.depth);
			break;

		case TextureType::Cube:
			OB_NOTIMPLEMENTED();
			break;

		default:
			OB_NOTIMPLEMENTED();
			break;
		}

		m_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		// リソース生成
		auto result = rDevice.getNative()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			m_state,
			nullptr,
			IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputFatalLog(result,TC("ID3D12Device::CreateCommittedResource()"));
		}

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);

		Utility::SetName(m_resource.Get(), getName());
    }

	//@―---------------------------------------------------------------------------
	//! @brief      IntColorの配列 から空のテクスチャを生成
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DirectX12RHI& rDevice, Size size, Span<IntColor> colors)
		: m_device(rDevice)
	{
		if (size.width <= 0 || size.height <= 0) {
			LOG_ERROR("Textureの生成に失敗。サイズが不正です。[size=({},{})]", size.width, size.height);
			return;
		}
		if (std::max(size.width, 1) * std::max(size.height, 1)* std::max(size.depth, 1) != colors.size()) {
			LOG_ERROR("Textureの生成に失敗。サイズとcolors.size()が一致していません。[size=({},{})]", size.width, size.height);
			return;
		}

		auto format = DXGI_FORMAT_R8G8B8A8_UNORM;

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 0;

		if (size.height == 0) {
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex1D(format, size.width);
		} else if (size.depth == 0) {
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, size.width, size.height);
		} else {
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex3D(format, size.width, size.height, size.depth);
		}

		m_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		// リソース生成
		auto result = rDevice.getNative()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			m_state,
			nullptr,
			IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
		}

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);


		result = m_resource->WriteToSubresource(
			0,
			nullptr,
			colors.data(),
			sizeof(IntColor)*size.width,
			sizeof(IntColor) * size.width*size.height
		);
		if (FAILED(result)) {
			Utility::OutputErrorLog(result, TC("ID3D12Resource::WriteToSubresource()"));
			return;
		}


		auto convertType = [](D3D12_RESOURCE_DIMENSION dimension) {
			switch (dimension) {
			case D3D12_RESOURCE_DIMENSION_TEXTURE1D:return TextureType::Texture1D;
			case D3D12_RESOURCE_DIMENSION_TEXTURE2D:return TextureType::Texture2D;
			case D3D12_RESOURCE_DIMENSION_TEXTURE3D:return TextureType::Texture3D;
			}
			return TextureType::Texture2D;
		};

		// Desc設定
		m_desc.name = Format(TC("Generated {}"), colors.empty()?IntColor::Black : colors[0]);
		m_desc.size = size;
		m_desc.type = convertType(resourceDesc.Dimension);
		m_desc.format = TypeConverter::Convert(format);
		m_desc.arrayNum = 1;
		m_desc.mipLevels = 0;


		Utility::SetName(m_resource.Get(), getName());
	}

	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャバイナリから生成
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DirectX12RHI& rDevice, BlobView blob,StringView name)
		: m_device(rDevice)
	{
		// 拡張子に合わせて読み込み
		HRESULT result;
		DirectX::TexMetadata metadata = {};
		DirectX::ScratchImage scratchImg = {};

		result = DirectX::LoadFromDDSMemory(blob.data(), blob.size(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImg);
		
		if (FAILED(result)) {
			Utility::OutputErrorLog(result, TC("DirectX::LoadFromDDSMemory()"));
			return;
		}
		
		//WriteToSubresourceで転送する用のヒープ設定
		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			(UINT16)metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);
		

		m_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		ComPtr<ID3D12Resource> resource;
		
		result = m_device.getNative()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			m_state,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
		
		if (FAILED(result)) {
			Utility::OutputErrorLog(result, TC("DirectX::LoadFromDDSMemory()"));
			return;
		}
		
		
		// GPUにデータ転送
		auto img = scratchImg.GetImage(0, 0, 0);
		
		result = resource->WriteToSubresource(
			0,
			nullptr,               
			img->pixels,           
			(UINT)img->rowPitch,   
			(UINT)img->slicePitch  
		);
		if (FAILED(result)) {
			Utility::OutputErrorLog(result, TC("ID3D12Resource::WriteToSubresource()"));
			return;
		}
		

		auto convertType = [](DirectX::TEX_DIMENSION dimension) {
			switch (dimension) {
			case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:return TextureType::Texture1D;
			case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:return TextureType::Texture2D;
			case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:return TextureType::Texture3D;
			}
			return TextureType::Texture2D;
		};
		
		// Desc設定
		m_desc.name = name;
		m_desc.size = {(s32)metadata.width,(s32)metadata.height,(s32)metadata.depth};
		m_desc.type = convertType(metadata.dimension);
		m_desc.format = TypeConverter::Convert(metadata.format);
		m_desc.arrayNum = (s32)metadata.arraySize;
		m_desc.mipLevels = (s32)metadata.mipLevels;

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);
		m_resource = resource;

		Utility::SetName(m_resource.Get(), getName());
	}


	//@―---------------------------------------------------------------------------
	//! @brief       RenderTextureDesc からRenderTextureを生成
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DirectX12RHI& rDevice, const RenderTextureDesc& desc)
		: m_device(rDevice)
		, m_renderDesc(desc)
	{
		m_desc.name = desc.name;
		m_desc.size = desc.size;
		m_desc.type = TextureType::Texture2D;
		m_desc.format = desc.format;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 0;

		createRenderTexture();

		if (m_renderDesc.display) {
			m_renderDesc.display->addEventListener(m_hUpdateDisplay, { *this,&TextureImpl::onUpdateDisplay });
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief      SwapChainのリソースからRenderTextureを生成
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DirectX12RHI& rDevice, const ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES state,StringView name)
		: m_device(rDevice)
		, m_resource(resource)
		, m_state(state)
	{

		if (!resource)
			return;

		// SRV生成
		{
			rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);

			D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			viewDesc.Format = resource->GetDesc().Format;
			viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			viewDesc.Texture2D.MipLevels = 1;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hSRV.getCpuHandle();
			rDevice.getNative()->CreateShaderResourceView(resource.Get(), &viewDesc, handle);
		}

		// RTV生成
		{
			rDevice.allocateHandle(DescriptorHeapType::RTV, m_hRTV, 1);

			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Format = resource->GetDesc().Format;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hRTV.getCpuHandle();
			rDevice.getNative()->CreateRenderTargetView(resource.Get(), &viewDesc, handle);
		}

		m_viewport = CD3DX12_VIEWPORT(resource.Get());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		auto resourceDesc = m_resource->GetDesc();

		m_desc.name = name;
		m_desc.size = { (s32)resourceDesc.Width,(s32)resourceDesc.Height, 0};
		m_desc.type = TextureType::Texture2D;
		m_desc.format = TypeConverter::Convert(resourceDesc.Format);
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 0;

		m_renderDesc.format = m_desc.format;
		m_renderDesc.size = m_desc.size;
		m_renderDesc.clear.color = Color::White;

		Utility::SetName(m_resource.Get(), getName());

	}

	void TextureImpl::createRenderTexture() {

		if (m_desc.size.width <= 0 || m_desc.size.height <= 0) {
			LOG_ERROR("RenderTextureの生成に失敗。サイズが不正です。[size=({},{})]", m_desc.size.width, m_desc.size.height);
			return;
		}

		// クリアカラー設定
		const FLOAT clearColor[4] = { m_renderDesc.clear.color.r,m_renderDesc.clear.color.g,m_renderDesc.clear.color.b,m_renderDesc.clear.color.a };
		auto format = TypeConverter::Convert(m_renderDesc.format);
		auto colorClearValue = CD3DX12_CLEAR_VALUE(format, clearColor);
		auto depthClearValue = CD3DX12_CLEAR_VALUE(format, m_renderDesc.clear.depth, m_renderDesc.clear.stencil);

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, m_renderDesc.size.width, m_renderDesc.size.height);
		m_state = D3D12_RESOURCE_STATE_COMMON;

		D3D12_CLEAR_VALUE* clearValue = nullptr;
		const bool isColor = !TextureFormatUtility::HasDepth(m_renderDesc.format);
		const bool isDepth = !isColor;
		if (isColor) {

			OB_ASSERT(!TextureFormatUtility::HasDepth(m_renderDesc.format), "カラーに非対応なフォーマットです。");
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			m_state = D3D12_RESOURCE_STATE_RENDER_TARGET;
			clearValue = &colorClearValue;

		}
		if (isDepth) {

			OB_ASSERT(TextureFormatUtility::HasDepth(m_renderDesc.format), "デプス・ステンシルに非対応なフォーマットです。");
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			m_state = D3D12_RESOURCE_STATE_DEPTH_WRITE;
			clearValue = &depthClearValue;

		}

		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


		// リソース生成

		ComPtr<ID3D12Resource> resource;

		auto result = m_device.getNative()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			m_state,
			clearValue,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
		}


		// SRV生成

		if (isColor) {
			m_device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);

			D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			viewDesc.Format = resource->GetDesc().Format;
			viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			viewDesc.Texture2D.MipLevels = 1;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hSRV.getCpuHandle();
			m_device.getNative()->CreateShaderResourceView(resource.Get(), &viewDesc, handle);
		}

		// RTV生成
		if (isColor) {

			m_device.allocateHandle(DescriptorHeapType::RTV, m_hRTV, 1);

			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Format = format;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hRTV.getCpuHandle();
			m_device.getNative()->CreateRenderTargetView(resource.Get(), &viewDesc, handle);

		}

		// DSV生成
		if (isDepth) {

			m_device.allocateHandle(DescriptorHeapType::DSV, m_hDSV, 1);

			D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
			viewDesc.Format = format;
			viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			viewDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hDSV.getCpuHandle();
			m_device.getNative()->CreateDepthStencilView(resource.Get(), &viewDesc, handle);

		}

		m_viewport = CD3DX12_VIEWPORT(resource.Get());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		m_resource = resource;

		// リソース名設定
		{
			WString wname;
			StringEncoder::Encode(m_desc.name, wname);
			m_resource->SetName(wname.c_str());
		}

		Utility::SetName(m_resource.Get(), getName());

	}

	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool TextureImpl::isValid()const{
		return !!m_resource;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      名前を取得
	//@―---------------------------------------------------------------------------
	const String& TextureImpl::getName()const {
		return m_desc.name;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      定義取得
	//@―---------------------------------------------------------------------------
	const TextureDesc& TextureImpl::desc()const{
		return m_desc;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      イベントリスナ追加
	//@―---------------------------------------------------------------------------
	void TextureImpl::addEventListener(TextureEventHandle& handle, TextureEventDelegate func) {
		m_notifier.add(handle, func);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      シェーダリソースビューを生成
	//@―---------------------------------------------------------------------------
	void TextureImpl::createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		D3D12_SHADER_RESOURCE_VIEW_DESC texDesc = {};
		texDesc.Format = m_resource->GetDesc().Format;
		texDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		switch (m_desc.type) {
		case TextureType::Texture1D:
			if (m_desc.arrayNum) {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
				texDesc.Texture1DArray.MipLevels = 1;
			} else {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
				texDesc.Texture1D.MipLevels = 1;
			}
			break;
		case TextureType::Texture2D:
			if (1<m_desc.arrayNum) {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				texDesc.Texture2DArray.MipLevels = 1;
			} else {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				texDesc.Texture2D.MipLevels = 1;
			}
			break;
		case TextureType::Texture3D:
			if (1 < m_desc.arrayNum) {
				OB_ABORT("Texture3Dは配列にできません。");
			} else {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
				texDesc.Texture3D.MipLevels = 1;
			}
			break;
		case TextureType::Cube:
			if (1 < m_desc.arrayNum) {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				texDesc.TextureCubeArray.MipLevels = 1;
			} else {
				texDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				texDesc.TextureCube.MipLevels = 1;
			}
			break;
		default:
			OB_ABORT("不明なテクスチャタイプ");
			break;
		}

		m_device.getNative()->CreateShaderResourceView(m_resource.Get(),&texDesc, handle);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  リソース遷移を追加
	//@―---------------------------------------------------------------------------
	bool TextureImpl::addResourceTransition(D3D12_RESOURCE_BARRIER& barrier,D3D12_RESOURCE_STATES state,s32 subresource) {

		if (m_state == state)
			return false;

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_resource.Get();
		barrier.Transition.Subresource = subresource==-1?D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES:subresource;
		barrier.Transition.StateBefore = m_state;
		barrier.Transition.StateAfter = state;

		if (m_state == D3D12_RESOURCE_STATE_RENDER_TARGET && state == D3D12_RESOURCE_STATE_DEPTH_WRITE) {
			CallBreakPoint();
		}

		m_state = state;

		return true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  クリアコマンドを記録
	//@―---------------------------------------------------------------------------
	void TextureImpl::clear(ID3D12GraphicsCommandList* cmdList) {

		if (cmdList) {
			if (m_hRTV) {
				auto t = m_renderDesc.clear.color;
				FLOAT clearColor[4] = { t.r,t.g,t.b,t.a };
				cmdList->ClearRenderTargetView(m_hRTV.getCpuHandle(), clearColor, 0, nullptr);
			}
			if (m_hDSV) {
				FLOAT depth = m_renderDesc.clear.depth;
				UINT8 stencil = m_renderDesc.clear.stencil;

				D3D12_CLEAR_FLAGS clearFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
				cmdList->ClearDepthStencilView(m_hDSV.getCpuHandle(), clearFlags, depth, stencil, 0, nullptr);
			}
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief  ディスプレイの更新イベント
	//@―---------------------------------------------------------------------------
	void TextureImpl::onUpdateDisplay() {

		Size newSize = m_desc.size;
		TextureFormat format = m_desc.format;

		// TODO Displayフォーマット変更
		//m_desc.format =

		if (m_renderDesc.display) {

			// TODO HDR変更対応
			//format = m_renderDesc.display->getDesc().format;

			newSize = m_renderDesc.display->getDesc().size;

		}

		if (m_desc.format == format && m_desc.size == newSize)
			return;

		m_desc.format = m_renderDesc.format = format;
		m_desc.size = m_renderDesc.size = newSize;

		createRenderTexture();

		m_notifier.invoke();
	}

}// ob::rhi::dx12
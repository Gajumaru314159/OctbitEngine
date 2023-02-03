//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "TextureImpl.h"
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <DirectXTex.h>


namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    TextureImpl::TextureImpl(DeviceImpl& rDevice, const TextureDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{

		auto format = TypeConverter::Convert(desc.format);
		
		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc{};
		D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;
		
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

		resourceDesc.DepthOrArraySize = std::max(desc.arrayNum, 1);
		resourceDesc.MipLevels = std::max(desc.mipLevels,0);


		const FLOAT color[4] = { 1.f,1.f,1.f,1.f };
		auto clearValue = CD3DX12_CLEAR_VALUE(format, color);

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		auto result = rDevice.getNative()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputFatalLog(result,TC("ID3D12Device::CreateCommittedResource()"));
		}

#ifdef OB_DEBUG
		{
			m_resource->SetName(TEXT("Texture"));
		}
#endif

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);

    }


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DeviceImpl& rDevice, BlobView blob)
		: m_device(rDevice)
	{
		// 拡張子に合わせて読み込み
		HRESULT result;
		DirectX::TexMetadata metadata = {};
		DirectX::ScratchImage scratchImg = {};

		result = DirectX::LoadFromDDSMemory(blob.data(), blob.size(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImg);
		
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("DirectX::LoadFromDDSMemory()"));
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
		
		ComPtr<ID3D12Resource> resource;
		
		result = m_device.getNative()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
		
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("DirectX::LoadFromDDSMemory()"));
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
			Utility::outputErrorLog(result, TC("ID3D12Resource::WriteToSubresource()"));
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
		resource->SetName(L"aaaa");

		// Desc設定
		m_desc.size = {(s32)metadata.width,(s32)metadata.height,(s32)metadata.depth};
		m_desc.type = convertType(metadata.dimension);
		m_desc.format = TypeConverter::Convert(metadata.format);
		m_desc.arrayNum = (s32)metadata.arraySize;
		m_desc.mipLevels = (s32)metadata.mipLevels;

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);
		m_resource = resource;

	}


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DeviceImpl& rDevice, const RenderTextureDesc& desc)
		: m_device(rDevice)
		, m_renderDesc(desc)
	{
		// クリアカラー設定
		const FLOAT clearColor[4] = { desc.clear.color.r,desc.clear.color.g,desc.clear.color.b,desc.clear.color.a };
		auto format = TypeConverter::Convert(desc.format);
		auto colorClearValue = CD3DX12_CLEAR_VALUE(format, clearColor);
		auto depthClearValue = CD3DX12_CLEAR_VALUE(format, desc.clear.depth, desc.clear.stencil);

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
		D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;

		D3D12_CLEAR_VALUE* clearValue = nullptr;
		const bool isColor = !TextureFormatUtility::HasDepth(desc.format);
		const bool isDepth = !isColor;

		if (isColor) {

			OB_ASSERT(!TextureFormatUtility::HasDepth(desc.format), "カラーに非対応なフォーマットです。");
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			resourceStates = D3D12_RESOURCE_STATE_RENDER_TARGET;
			clearValue = &colorClearValue;

		}
		if (isDepth) {

			OB_ASSERT(TextureFormatUtility::HasDepth(desc.format), "デプス・ステンシルに非対応なフォーマットです。");
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			resourceStates = D3D12_RESOURCE_STATE_DEPTH_WRITE;
			clearValue = &depthClearValue;

		}

		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


		// リソース生成

		ComPtr<ID3D12Resource> resource;

		auto result = rDevice.getNative()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			resourceStates,
			clearValue,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
		}

		
		// SRV生成

		if (isColor) {
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
		if (isColor) {

			rDevice.allocateHandle(DescriptorHeapType::RTV, m_hRTV, 1);

			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Format = format;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hRTV.getCpuHandle();
			rDevice.getNative()->CreateRenderTargetView(resource.Get(), &viewDesc, handle);

		}

		// DSV生成
		if (isDepth) {

			rDevice.allocateHandle(DescriptorHeapType::DSV, m_hDSV, 1);

			D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
			viewDesc.Format = format;
			viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			viewDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hDSV.getCpuHandle();
			rDevice.getNative()->CreateDepthStencilView(resource.Get(), &viewDesc, handle);

		}

		m_viewport = CD3DX12_VIEWPORT(resource.Get());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		m_resource = resource;

		// リソース名設定
		{
			std::wstring wname;
			StringEncoder::Encode(m_desc.name, wname);
			m_resource->SetName(wname.c_str());
		}

		m_desc.size = desc.size;
		m_desc.type = TextureType::Texture2D;
		m_desc.format = desc.format;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 0;

	}


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	TextureImpl::TextureImpl(DeviceImpl& rDevice, const ComPtr<ID3D12Resource>& resource)
		: m_device(rDevice)
		, m_resource(resource)
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

		m_desc.size = { (s32)resourceDesc.Width,(s32)resourceDesc.Height, 0};
		m_desc.type = TextureType::Texture2D;
		m_desc.format = TypeConverter::Convert(resourceDesc.Format);
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 0;

		m_renderDesc.format = m_desc.format;
		m_renderDesc.size = m_desc.size;
		m_renderDesc.clear.color = Color::White;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool TextureImpl::isValid()const{
		return !!m_resource;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  サイズ
	//@―---------------------------------------------------------------------------
	Size TextureImpl::size()const {
		return m_desc.size;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ・フォーマットを取得
	//@―---------------------------------------------------------------------------
	TextureFormat TextureImpl::format()const {
		return m_desc.format;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ミップレベルを取得
	//@―---------------------------------------------------------------------------
	s32 TextureImpl::mipLevels()const {
		return m_desc.mipLevels;
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
	//! @brief  名前変更時
	//@―---------------------------------------------------------------------------
	void TextureImpl::onNameChanged() {
		Utility::setName(m_resource.Get(), getName());
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

}// ob::rhi::dx12
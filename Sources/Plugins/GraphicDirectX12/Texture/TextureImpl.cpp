﻿//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "TextureImpl.h"
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>


namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    TextureImpl::TextureImpl(DeviceImpl& rDevice, const TextureDesc& desc, StringView name) {

		auto format = Utility::convertTextureFormat(desc.format);
		
		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc{};
		D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;
		
		switch (desc.type) {
		case TextureType::Texture1D:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex1D(format, desc.size.width);
			resourceStates |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			break;

		case TextureType::Texture2D:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			resourceStates |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			break;

		case TextureType::Texture3D:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex3D(format, desc.size.width, desc.size.height,desc.size.depth);
			resourceStates |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			break;

		case TextureType::Cube:
			OB_NOTIMPLEMENTED();
			break;

		case TextureType::RenderTarget:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			resourceStates |= D3D12_RESOURCE_STATE_RENDER_TARGET;
			break;

		case TextureType::DeptthStencil:
			OB_CHECK_ASSERT_EX(TextureFormatUtility::HasDepth(desc.format), "デプス・ステンシルに非対応なフォーマットです。");
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			resourceStates |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
			break;
		default:
			OB_NOTIMPLEMENTED();
			break;
		}

		if (desc.arrayNum != 1) {
			resourceDesc.DepthOrArraySize = desc.arrayNum;
		}
		if (desc.mipLevel != 1) {
			resourceDesc.DepthOrArraySize = desc.mipLevel;
		}


		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;


		ComPtr<ID3D12Resource> resource;

		auto result = rDevice.getNativeDevice()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			resourceStates,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(result)) {
			LOG_FATAL_EX("Graphic", "ID3D12Device::CreateCommittedResourceに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
		}

		m_resource = resource;

    }


	//@―---------------------------------------------------------------------------
	//! @brief      リソースを持っているか
	//@―---------------------------------------------------------------------------
	bool TextureImpl::hasResource()const {
		return !m_resource;
	}


    //@―---------------------------------------------------------------------------
    //! @brief      シェーダリソースビューを生成
    //@―---------------------------------------------------------------------------
    bool TextureImpl::createSRV(ID3D12Device& rNativeDevice, D3D12_CPU_DESCRIPTOR_HANDLE& handle) {
		if (hasResource() == false)return false;
		OB_NOTIMPLEMENTED();

		auto& desc = m_resource->GetDesc();

		D3D12_SHADER_RESOURCE_VIEW_DESC texDesc = {};
		texDesc.Format = desc.Format;
		texDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		// TODO: テクスチャリソース定義
		texDesc.ViewDimension = Utility::getSrvDimention(desc);

		/*
		switch (texDesc.ViewDimension) {
		case D3D12_SRV_DIMENSION_TEXTURE1D:
			texDesc.Texture1D.MipLevels = 1;
			break;
		case D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
			texDesc.Texture1DArray.MipLevels = 1;
			break;
		case D3D12_SRV_DIMENSION_TEXTURE2D:
			texDesc.Texture2D.MipLevels = 1;
			break;
		case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
			texDesc.Texture2DArray.MipLevels = 1;
			break;
		case D3D12_SRV_DIMENSION_TEXTURE2DMS:
			break;
		case D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY:
			texDesc.Texture2DMSArray.ArraySize;
			texDesc.Texture2DMSArray.FirstArraySlice;
			break;
		case D3D12_SRV_DIMENSION_TEXTURE3D:
			texDesc.Texture3D.MipLevels = 1;
			break;
		case D3D12_SRV_DIMENSION_TEXTURECUBE:
			texDesc.TextureCube.MipLevels;// .MipLevels = 1;
			break;
		case D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
			texDesc.TextureCube.MipLevels;// .MipLevels = 1;
			break;
		}
		*/

		rNativeDevice.CreateShaderResourceView(m_resource.Get(), &texDesc, handle);
		handle.ptr += rNativeDevice.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		return 0;
    }

}// ob::graphic::dx12
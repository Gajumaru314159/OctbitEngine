//***********************************************************
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

    }


    //@―---------------------------------------------------------------------------
    //! @brief      シェーダリソースビューを取得
    //@―---------------------------------------------------------------------------
    ID3D12DescriptorHeap* TextureImpl::getSRV() {

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
		if (hasResource() == false)return -1;

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
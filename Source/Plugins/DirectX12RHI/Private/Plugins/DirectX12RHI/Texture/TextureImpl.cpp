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

	TextureFormat convertDXGIFormat(DXGI_FORMAT dxgi) {
		switch (static_cast<DXGI_FORMAT>(dxgi)) {
		case DXGI_FORMAT_R32G32B32A32_FLOAT:	return TextureFormat::RGBA32;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:	return TextureFormat::RGBA16;
		case DXGI_FORMAT_R8G8B8A8_UNORM:		return TextureFormat::RGBA8;
		case DXGI_FORMAT_R32G32B32_FLOAT:		return TextureFormat::RGB32;
		case DXGI_FORMAT_R32G32_FLOAT:			return TextureFormat::RG32;
		case DXGI_FORMAT_R16G16_FLOAT:			return TextureFormat::RG16;
		case DXGI_FORMAT_R8G8_UNORM:			return TextureFormat::RG8;
		case DXGI_FORMAT_R32_FLOAT:				return TextureFormat::R32;
		case DXGI_FORMAT_R16_FLOAT:				return TextureFormat::R16;
		case DXGI_FORMAT_R8_UNORM:				return TextureFormat::R8;
		case DXGI_FORMAT_R10G10B10A2_UNORM:		return TextureFormat::R10G10B10A2;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:	return TextureFormat::D32S8;
		case DXGI_FORMAT_D32_FLOAT:				return TextureFormat::D32;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:		return TextureFormat::D24S8;
		case DXGI_FORMAT_D16_UNORM:				return TextureFormat::D16;
		case DXGI_FORMAT_BC1_UNORM:				return TextureFormat::BC1;
		case DXGI_FORMAT_BC2_UNORM:				return TextureFormat::BC2;
		case DXGI_FORMAT_BC3_UNORM:				return TextureFormat::BC3;
		case DXGI_FORMAT_BC4_UNORM:				return TextureFormat::BC4;
		case DXGI_FORMAT_BC5_UNORM:				return TextureFormat::BC5;
		case DXGI_FORMAT_BC6H_UF16:				return TextureFormat::BC6H;
		case DXGI_FORMAT_BC7_UNORM:				return TextureFormat::BC7;
		case DXGI_FORMAT_BC1_UNORM_SRGB:		return TextureFormat::BC1_SRGB;
		case DXGI_FORMAT_BC2_UNORM_SRGB:		return TextureFormat::BC2_SRGB;
		case DXGI_FORMAT_BC3_UNORM_SRGB:		return TextureFormat::BC3_SRGB;
		case DXGI_FORMAT_BC7_UNORM_SRGB:		return TextureFormat::BC7_SRGB;
		}
		return TextureFormat::Unknown;
	}




    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    TextureImpl::TextureImpl(DeviceImpl& rDevice, const TextureDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{

		auto format = TypeConverter::Convert(desc.format);
		D3D12_CLEAR_VALUE* pClearValue=nullptr;

		const FLOAT clearColor[4] = { desc.color.r,desc.color.g,desc.color.b,desc.color.a};
		auto colorClearValue = CD3DX12_CLEAR_VALUE(format, clearColor);
		auto depthClearValue = CD3DX12_CLEAR_VALUE(format, 1.0f, 0);
		
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
			pClearValue = &colorClearValue;
			break;

		case TextureType::DepthStencil:
			OB_ASSERT(TextureFormatUtility::HasDepth(desc.format), "デプス・ステンシルに非対応なフォーマットです。");
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, desc.size.width, desc.size.height);
			resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			resourceStates |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
			pClearValue = &depthClearValue;
			break;
		default:
			OB_NOTIMPLEMENTED();
			break;
		}

		if (0 < desc.arrayNum) {
			resourceDesc.DepthOrArraySize = desc.arrayNum;
		}
		if (0 < desc.mipLevels) {
			resourceDesc.MipLevels = desc.mipLevels;
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
			Utility::OutputFatalLog(result,TC("ID3D12Device::CreateCommittedResource()"));
		}

#ifdef OB_DEBUG
		{
			resource->SetName(TEXT("Texture"));
		}
#endif

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);

		m_resource = resource;

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
		
		result = resource->WriteToSubresource(0,
			nullptr,               //全領域へコピー
			img->pixels,           //元データアドレス
			(UINT)img->rowPitch,   //1ラインサイズ
			(UINT)img->slicePitch  //全サイズ
		);
		if (FAILED(result)) {
			Utility::outputErrorLog(result, TC("DirectX::LoadFromDDSMemory()"));
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

		m_desc.size = {(s32)metadata.width,(s32)metadata.height,(s32)metadata.depth};
		m_desc.type = convertType(metadata.dimension);
		m_desc.format = convertDXGIFormat(metadata.format);
		m_desc.arrayNum = (s32)metadata.arraySize;
		m_desc.mipLevels = (s32)metadata.mipLevels;
		m_desc.color = Color::White;

		rDevice.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_hSRV, 1);
		m_resource = resource;

	}


	//@―---------------------------------------------------------------------------
	//! @brief      デストラクタ
	//@―---------------------------------------------------------------------------
	TextureImpl::~TextureImpl() {

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
		case TextureType::RenderTarget:
		case TextureType::DepthStencil:
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

}// ob::rhi::dx12
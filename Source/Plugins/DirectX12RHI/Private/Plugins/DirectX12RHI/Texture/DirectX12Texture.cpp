//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "DirectX12Texture.h"
#include <Plugins/DirectX12RHI/DirectX12Device.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/Command/ResourceStateCache.h>
#include <DirectXTex.h>


namespace ob::rhi {

	//! @brief D3D12_RESOURCE_DESCを構築する
	static D3D12_RESOURCE_DESC CreateResourceDesc(TextureType type,TextureFormat format,Size size,s32 mipLevel,s32 arrayNum,StringView name) {

		auto nativeFormat = TypeConverter::Convert(format);

		// DepthOrArraySize は、1 から、特定のフィーチャー レベルとテクスチャ ディメンションでサポートされる最大ディメンションの間である必要があります。 
		// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_resource_desc

		switch (type) {
		case TextureType::Texture1D:
			return CD3DX12_RESOURCE_DESC::Tex1D(nativeFormat, size.width, std::max(1, arrayNum), mipLevel);
		case TextureType::Texture2D:
			return CD3DX12_RESOURCE_DESC::Tex2D(nativeFormat, size.width, size.height, std::max(1, arrayNum), mipLevel);
		case TextureType::Texture3D:
			if(0 < arrayNum) LOG_ERROR("Texture3Dは配列に対応していません [name={}]", name);
			return CD3DX12_RESOURCE_DESC::Tex3D(nativeFormat, size.width, size.height, size.depth, mipLevel);
		case TextureType::Cube:
			return CD3DX12_RESOURCE_DESC::Tex2D(nativeFormat, size.width, size.height, 6 * std::max(1, arrayNum), mipLevel);
		default:
			LOG_ERROR("不明なテクスチャタイプです [name={}]", name);
			return {};
		}
	}

	//! @brief SizeからTextureTypeに変換 (Texture::Cube非対応) 
	static TextureType TextureTypeFrom(Size size) {
		if (size.height == 0) return TextureType::Texture1D;
		if (size.depth == 0) return TextureType::Texture2D;
		return TextureType::Texture3D;
	}

	//! @brief TEX_DIMENSIONからTextureTypeに変換 (Texture::Cube非対応) 
	static TextureType TextureTypeFrom(DirectX::TEX_DIMENSION dimension) {
		switch (dimension) {
		case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:return TextureType::Texture1D;
		case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:return TextureType::Texture2D;
		case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:return TextureType::Texture3D;
		}
		return TextureType::Texture2D;
	};




    //! @brief      TextureDesc から空のテクスチャを生成
    DirectX12Texture::DirectX12Texture(DirectX12Device& device, const TextureDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		// バリデート
		if (!m_desc.isValid()) return;

		// 初期ステート
		m_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc(m_desc.type,m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto result = device.getNative()->CreateCommittedResource(&heapProps,D3D12_HEAP_FLAG_NONE,&resourceDesc,m_state,nullptr,IID_PPV_ARGS(m_resource.GetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputErrorLog(result,"ID3D12Device::CreateCommittedResource()");
			return;
		}

		initialize();
    }


	//! @brief      IntColorの配列 から空のテクスチャを生成
	DirectX12Texture::DirectX12Texture(DirectX12Device& device, StringView name, TextureType type,Size size, Span<const IntColor> colors)
		: m_device(device)
	{
		// Desc設定
		m_desc.name = name;
		m_desc.size = size;
		m_desc.type = type;
		m_desc.format = TextureFormat::RGBA8;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (!m_desc.isValid()) return;

		if (std::max(size.width, 1) * std::max(size.height, 1) * std::max(size.depth, 1) != colors.size()) {
			LOG_ERROR("Textureの生成に失敗。サイズとcolors.size()が一致していません。[size={}, name={}]", size, name);
			return;
		}

		// 初期ステート
		m_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc(m_desc.type, m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// リソース生成
		ComPtr<ID3D12Resource> resource;
		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		auto result = device.getNative()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, m_state, nullptr, IID_PPV_ARGS(resource.GetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputErrorLog(result, "ID3D12Device::CreateCommittedResource()");
			return;
		}

		// データ書き込み
#if 1
		UINT SrcRowPitch = sizeof(IntColor) * size.width;
		UINT SrcDepthPitch = sizeof(IntColor) * size.width * size.height;
		result = resource->WriteToSubresource(0,nullptr,colors.data(), SrcRowPitch, SrcDepthPitch);
		if (FAILED(result)) {
			Utility::OutputErrorLog(result, "ID3D12Resource::WriteToSubresource()");
			return;
		}
#else
		// TODO TextureUploader対応
		FixedVector<TextureUploader::Subresource, 20> subresources;

		for (s32 array = 0; array < resourceDesc.DepthOrArraySize; ++array) {
			size_t offset = array * m_desc.size.volume() * sizeof(IntColor);
			auto& subresource = subresources.emplace_back();
			subresource.rowPitch = m_desc.size.width * sizeof(IntColor);
			subresource.slicePitch = subresource.rowPitch * m_desc.size.height;
			subresource.data = BlobView(GetOffsetPtr(colors.data(),offset), subresource.slicePitch);
		}

		m_device.getTextureUploader().add(resource, subresources);
#endif

		m_resource = resource;

		initialize();

	}


	//! @brief      テクスチャバイナリから生成
	DirectX12Texture::DirectX12Texture(DirectX12Device& device, StringView name,BlobView blob)
		: m_device(device)
	{
		// 拡張子に合わせて読み込み
		HRESULT result = 0;
		DirectX::TexMetadata metadata = {};
		DirectX::ScratchImage scratchImg = {};

		result = DirectX::LoadFromDDSMemory(blob.data(), blob.size(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImg);
		if (FAILED(result)) result = DirectX::LoadFromWICMemory(blob.data(), blob.size(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
		if (FAILED(result)) result = DirectX::LoadFromTGAMemory(blob.data(), blob.size(), &metadata, scratchImg);
		if (FAILED(result)) result = DirectX::LoadFromHDRMemory(blob.data(), blob.size(), &metadata, scratchImg);
				
		if (FAILED(result)) {
			return;
		}

		// Desc設定
		m_desc.name = name;
		m_desc.size = { (s32)metadata.width,(s32)metadata.height,(s32)metadata.depth };
		m_desc.type = TextureTypeFrom(metadata.dimension);
		m_desc.format = TypeConverter::Convert(metadata.format);
		m_desc.arrayNum = (s32)metadata.arraySize;
		m_desc.mipLevels = (s32)metadata.mipLevels;
		if (m_desc.arrayNum == 1) m_desc.arrayNum = 0; // 要素数1のTextureArrayはddsからは読み込めない

		// バリデート
		if (!m_desc.isValid()) return;
		
		// 初期ステート
		m_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		// リソース生成
		ComPtr<ID3D12Resource> resource;
		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		auto resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, (UINT16)metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize, (UINT16)metadata.mipLevels);
		result = m_device.getNative()->CreateCommittedResource(&heapProps,D3D12_HEAP_FLAG_NONE,&resourceDesc,m_state,nullptr,IID_PPV_ARGS(resource.GetAddressOf()));
		
		if (FAILED(result)) {
			Utility::OutputErrorLog(result, "DirectX::LoadFromDDSMemory()");
			return;
		}
		
#if 1
		// GPUにデータ転送
		// TODO WriteToSubresourceはUMA向けなのでNUMAの場合はCopyTextureRegionで転送する
		for (s32 i = 0; i < metadata.mipLevels; ++i) {
			auto img = scratchImg.GetImage(i, 0, 0);

			result = resource->WriteToSubresource(
				i,
				nullptr,
				img->pixels,
				(UINT)img->rowPitch,
				(UINT)img->slicePitch
			);
			if (FAILED(result)) {
				Utility::OutputErrorLog(result, "ID3D12Resource::WriteToSubresource()");
				return;
			}
		}
#else
		FixedVector<DirectX12TextureUploader::Subresource, 20> subresources;

		for (s32 array = 0; array < std::max<s32>(m_desc.arrayNum,1); ++array) {
			for (s32 depth = 0; depth < std::max<s32>(m_desc.size.depth, 1); ++depth) {
				for (s32 mipLevel = 0; mipLevel < resourceDesc.MipLevels; ++mipLevel) {

					auto img = scratchImg.GetImage(mipLevel, array, depth);

					auto& subresource = subresources.emplace_back();
					subresource.data = BlobView(img->pixels,img->slicePitch);
					subresource.rowPitch = img->rowPitch;
					subresource.slicePitch = img->slicePitch;
				}
			}
		}

		m_device.getTextureUploader().add(resource, subresources);
#endif

		m_resource = resource;

		initialize();

	}


	//! @brief      ベースのテクスチャを指定して異なるビューを持つテクスチャを作成
	DirectX12Texture::DirectX12Texture(DirectX12Device& device, const TextureViewDesc& desc)
		: m_device(device)
	{
		auto base = desc.base.cast<DirectX12Texture>();
		if (!base) {
			LOG_ERROR("ベーステクスチャが指定されていません");
			return;
		}

		m_desc = base->m_desc;
		m_viewDesc = desc;

		device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_handle, 1);

		auto flags = m_desc.flags;

		switch (desc.type) {
		case TextureViewType::Texture:
			if (flags & TextureFlag::ShaderResource) {
				createSRV(m_handle.getCpuHandle());
			} else {
				throw Exception("TextureFlag::ShaderResourceが指定されていないTextureをTextureViewType::Textureで使用しようとしました");
			}
			break;
		case TextureViewType::RWTexture:
			if (flags & TextureFlag::UnorderedAccess) {
				createUAV(m_handle.getCpuHandle(),0);
			} else {
				throw Exception("TextureFlag::UnorderedAccessが指定されていないTextureをTextureViewType::RWTextureで使用しようとしました");
			}
			break;
		}

		m_resource = base->m_resource;

		manage();

	}


	//! @brief       RenderTextureDesc からRenderTextureを生成
	DirectX12Texture::DirectX12Texture(DirectX12Device& device, const RenderTextureDesc& desc)
		: m_device(device)
		, m_renderDesc(desc)
	{
		m_desc.name = desc.name;
		m_desc.size = desc.size;
		m_desc.type = TextureType::Texture2D;
		m_desc.format = desc.format;
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		// バリデート
		if (!m_desc.isValid()) return;

		// 定義生成
		D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc(m_desc.type, m_desc.format, m_desc.size, m_desc.mipLevels, m_desc.arrayNum, m_desc.name);

		// クリアカラー設定
		const FLOAT clearColor[4] = { m_renderDesc.clear.color.r,m_renderDesc.clear.color.g,m_renderDesc.clear.color.b,m_renderDesc.clear.color.a };
		auto format = TypeConverter::Convert(m_renderDesc.format, true);
		auto colorClearValue = CD3DX12_CLEAR_VALUE(format, clearColor);
		auto depthClearValue = CD3DX12_CLEAR_VALUE(format, m_renderDesc.clear.depth, m_renderDesc.clear.stencil);

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


		// リソース生成
		ComPtr<ID3D12Resource> resource;
		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto result = device.getNative()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, m_state, nullptr, IID_PPV_ARGS(resource.GetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputErrorLog(result, "ID3D12Device::CreateCommittedResource()");
			return;
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
			viewDesc.Format = TypeConverter::Convert(m_renderDesc.format);
			viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			viewDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hDSV.getCpuHandle();
			m_device.getNative()->CreateDepthStencilView(resource.Get(), &viewDesc, handle);

		}

		m_resource = resource;
		m_viewport = CD3DX12_VIEWPORT(resource.Get());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		initialize();

	}


	//! @brief      SwapChainのリソースからRenderTextureを生成
	DirectX12Texture::DirectX12Texture(DirectX12Device& device, const ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES state,StringView name)
		: m_device(device)
		, m_resource(resource)
		, m_state(state)
	{

		if (!resource)
			return;

		// RTV生成
		{
			device.allocateHandle(DescriptorHeapType::RTV, m_hRTV, 1);

			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Format = resource->GetDesc().Format;

			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hRTV.getCpuHandle();
			device.getNative()->CreateRenderTargetView(resource.Get(), &viewDesc, handle);
		}

		m_viewport = CD3DX12_VIEWPORT(resource.Get());
		m_scissorRect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

		auto resourceDesc = m_resource->GetDesc();

		m_desc.name = name;
		m_desc.size = { (s32)resourceDesc.Width,(s32)resourceDesc.Height, 0};
		m_desc.type = TextureType::Texture2D;
		m_desc.format = TypeConverter::Convert(resourceDesc.Format);
		m_desc.arrayNum = 0;
		m_desc.mipLevels = 1;

		m_renderDesc.format = m_desc.format;
		m_renderDesc.size = m_desc.size;
		m_renderDesc.clear.color = Color::White;

		Utility::SetName(m_resource.Get(), m_desc.name);

		manage();

	}

	//! @brief 共通初期化処理
	void DirectX12Texture::initialize() {

		m_viewDesc.type = TextureViewType::Texture;

		// デフォルトSRVを割り当て
		// TODO SRVをつくってはいけないパターンがないか確認する
		m_device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_handle, 1);
		createSRV(m_handle.getCpuHandle());

		// リソース名を設定
		Utility::SetName(m_resource.Get(), m_desc.name);

		manage();
	}


	//! @brief      SRVを生成
	void DirectX12Texture::createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		// Depthをカラーテクスチャとして使用する場合は別の型に設定する必要がある
		if (TextureFormatUtility::HasColor(m_desc.format)) {
			desc.Format = m_resource->GetDesc().Format;
		} else {
			desc.Format = TypeConverter::ConvertDepthAsColor(m_desc.format);
		}

		// エンジンの仕様では0以下がすべてのミップを表すがD3D12では-1を指定する
		auto mipLevels = m_desc.mipLevels <= 0 ? -1 : m_desc.mipLevels;

		// NOTE 特定のスライスを指定する場合は追加の引数が必要
		switch (m_desc.type) {
		case TextureType::Texture1D:
			if (0 < m_desc.arrayNum) {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipLevels = mipLevels;
				desc.Texture1DArray.ArraySize = m_desc.arrayNum;
			} else {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipLevels = mipLevels;
			}
			break;
		case TextureType::Texture2D:
			if (0 < m_desc.arrayNum) {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipLevels = mipLevels;
				desc.Texture2DArray.ArraySize = m_desc.arrayNum;
			} else {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipLevels = mipLevels;
			}
			break;
		case TextureType::Texture3D:
			if (0 < m_desc.arrayNum) {
				OB_ABORT("Texture3Dは配列にできません。IsInvalid()の条件を修正してください。");
			} else {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipLevels = mipLevels;
			}
			break;
		case TextureType::Cube:
			if (0 < m_desc.arrayNum) {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				desc.TextureCubeArray.MipLevels = mipLevels;
				desc.TextureCubeArray.NumCubes = m_desc.arrayNum;
			} else {
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				desc.TextureCube.MipLevels = mipLevels;
			}
			break;
		default:
			OB_ABORT("不明なテクスチャタイプ");
			break;
		}

		m_device.getNative()->CreateShaderResourceView(m_resource.Get(),&desc, handle);
	}


	//! @brief      UAVを生成
	void DirectX12Texture::createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle,s32 slice)const {

		// TODO Sliceの扱いを調査
		OB_NOTIMPLEMENTED();

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};

		// Depthをカラーテクスチャとして使用する場合は別の型に設定する必要がある
		if (TextureFormatUtility::HasColor(m_desc.format)) {
			desc.Format = m_resource->GetDesc().Format;
		} else {
			desc.Format = TypeConverter::ConvertDepthAsColor(m_desc.format);
		}

		// NOTE 特定のスライスを指定する場合は追加の引数が必要
		switch (m_desc.type) {
		case TextureType::Texture1D:
			if (m_desc.arrayNum) {
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = slice;
				desc.Texture1DArray.FirstArraySlice = 0;
				desc.Texture1DArray.ArraySize = m_desc.arrayNum;
			}
			else {
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = slice;
			}
			break;
		case TextureType::Texture2D:
			if (1 < m_desc.arrayNum) {
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = slice;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.ArraySize = m_desc.arrayNum;
				desc.Texture2DArray.PlaneSlice = 0;
			}
			else {
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = slice;
				desc.Texture2D.PlaneSlice = 0;
			}
			break;
		case TextureType::Texture3D:
			if (1 < m_desc.arrayNum) {
				OB_ABORT("Texture3Dは配列にできません。");
			}
			else {
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = slice;
				desc.Texture3D.FirstWSlice = 0;
				desc.Texture3D.WSize = 0;
			}
			break;
		case TextureType::Cube:
			OB_ABORT("TextureCubeはUAVに使用できません");
			break;
		default:
			OB_ABORT("不明なテクスチャタイプ");
			break;
		}

		// TODO pCounterResource の調査
		m_device.getNative()->CreateUnorderedAccessView(m_resource.Get(),nullptr, &desc, handle);
	}


	//! @brief  リソース遷移を追加
	//! @param  subresource -1で全てのサブリソースを指定
	bool DirectX12Texture::addResourceTransition(D3D12_RESOURCE_BARRIER& barrier,D3D12_RESOURCE_STATES state,s32 subresource) {

		if (m_state == state)
			return false;

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_resource.Get();
		barrier.Transition.Subresource = subresource==-1?D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES:subresource;
		barrier.Transition.StateBefore = m_state;
		barrier.Transition.StateAfter = state;

		m_state = state;

		return true;
	}


	//! @brief  クリアコマンドを記録
	void DirectX12Texture::clear(ID3D12GraphicsCommandList* cmdList) {

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

}// ob::rhi
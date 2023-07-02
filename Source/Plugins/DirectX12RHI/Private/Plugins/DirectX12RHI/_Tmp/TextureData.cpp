﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#if 0
#include <Framework/RHI/TextureData.h>
#include <Framework/RHI/Private/DDSFormat.h>
#include <Framework/RHI/Private/DXGIFormat.h>
#include <Framework/Core/File/BinaryReader.h>
#include <Framework/Core/File/FileStream.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ob::rhi
{

	//@―---------------------------------------------------------------------------
	//! @brief  マジックコードをチェック
	//@―---------------------------------------------------------------------------
	template<size_t N, typename T = unsigned int>
	constexpr T checkMagic(const byte(&bytes)[N], const char(&code)[N + 1]) {
		for (size_t i = 0; i < N - 1; ++i) {
			if (code[i] != bytes[i])return false;
		}
		return true;
	}

	auto calcNextPixelCount(u32 width, u32 height) {
		return std::max<u32>(1, ((width + 3) / 4)) * std::max<u32>(1, ((height + 3) / 4));
	}



	TextureFormat convertDXGIFormat(u32 dxgi) {
		switch (static_cast<DXGI_FORMAT>(dxgi)) {
		case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:	return TextureFormat::RGBA32;
		case DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT:	return TextureFormat::RGBA16;
		case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:		return TextureFormat::RGBA8;
		case DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT:		return TextureFormat::RGB32;
		//case :return TextureFormat::RGB8
		case DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT:			return TextureFormat::RG32;
		case DXGI_FORMAT::DXGI_FORMAT_R16G16_FLOAT:			return TextureFormat::RG16;
		case DXGI_FORMAT::DXGI_FORMAT_R8G8_UNORM:			return TextureFormat::RG8;
		case DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT:			return TextureFormat::R32;
		case DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT:			return TextureFormat::R16;
		case DXGI_FORMAT::DXGI_FORMAT_R8_UNORM:				return TextureFormat::R8;
		case DXGI_FORMAT::DXGI_FORMAT_R10G10B10A2_UNORM:	return TextureFormat::R10G10B10A2;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:	return TextureFormat::D32S8;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:			return TextureFormat::D32;
		case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:	return TextureFormat::D24S8;
		case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:			return TextureFormat::D16;
		case DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM:			return TextureFormat::BC1;
		case DXGI_FORMAT::DXGI_FORMAT_BC2_UNORM:			return TextureFormat::BC2;
		case DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM:			return TextureFormat::BC3;
		case DXGI_FORMAT::DXGI_FORMAT_BC4_UNORM:			return TextureFormat::BC4;
		case DXGI_FORMAT::DXGI_FORMAT_BC5_UNORM:			return TextureFormat::BC5;
		case DXGI_FORMAT::DXGI_FORMAT_BC6H_UF16:			return TextureFormat::BC6H;
		case DXGI_FORMAT::DXGI_FORMAT_BC7_UNORM:			return TextureFormat::BC7;
		case DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM_SRGB:		return TextureFormat::BC1_SRGB;
		case DXGI_FORMAT::DXGI_FORMAT_BC2_UNORM_SRGB:		return TextureFormat::BC2_SRGB;
		case DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM_SRGB:		return TextureFormat::BC3_SRGB;
		case DXGI_FORMAT::DXGI_FORMAT_BC7_UNORM_SRGB:		return TextureFormat::BC7_SRGB;
		}
		return TextureFormat::Unknown;
	}



	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	TextureData::TextureData(const Path& path)
		: m_size(0, 0)
		, m_format(TextureFormat::RGBA8)
		, m_type(TextureType::Texture2D)
		, m_arrayNum(0)
	{

		enum class DDSErrorCode {
			None,
			InvalidData,
		};
		DDSErrorCode errorCode = DDSErrorCode::None;

		FileStream fs(path);
		bool isDDS = true;
		if (fs) {
			BinaryReader reader(fs);

			// ヘッダ読み込み
			DDS_HEADER header;
			if (!reader.read(header))return;
			if (!checkMagic(header.magic, "DDS "))return;


			u32 width = (header.flags & DDSD_WIDTH) ? header.width : 1;
			u32 height = (header.flags & DDSD_HEIGHT) ? header.height : 1;
			u32 depth = (header.flags & DDSD_DEPTH) ? header.depth : 1;
			u32 mipLevels = (header.flags & DDSD_MIPMAPCOUNT) ? std::max<u32>(header.mipMapCount, 1) : 1;
			u32 arraySize = 1;
			u32 faceCount = 1;
			TextureFormat format=TextureFormat::Unknown;
			TextureType type = TextureType::Texture2D;

			Char magic2[] = { header.ddspf.fourCC[0] ,header.ddspf.fourCC[1],header.ddspf.fourCC[2] ,header.ddspf.fourCC[3] ,'\0' };
			if (header.ddspf.flags & DDPF_FOURCC) {
				if (checkMagic(header.ddspf.fourCC, "DX10")) {

					DDS_HEADER_DXT10 exHeader;
					if (!reader.read(exHeader))return;
					arraySize = exHeader.arraySize;
					format = convertDXGIFormat(exHeader.dxgiFormat);

					switch (exHeader.resourceDimension)
					{
					case DDS_DIMENSION_TEXTURE1D:
						type = TextureType::Texture1D;
						break;
					case DDS_DIMENSION_TEXTURE2D:
						if (exHeader.miscFlag & DDS_MISC_FLAG_TEXTURECUBE) {
							type = TextureType::Cube;
							arraySize *= 6;
						} else {
							type = TextureType::Texture2D;
						}
						break;
					case DDS_DIMENSION_TEXTURE3D:
						type = TextureType::Texture3D;
						break;
					}

				} else if (checkMagic(header.ddspf.fourCC, "DXT1")) {
					m_format = TextureFormat::BC1;
				} else if (checkMagic(header.ddspf.fourCC, "DXT2")) {
					m_format = TextureFormat::BC2;
				} else if (checkMagic(header.ddspf.fourCC, "DXT3")) {
					m_format = TextureFormat::BC3;
				} else if (checkMagic(header.ddspf.fourCC, "DXT4")) {
					m_format = TextureFormat::BC3;
				} else if (checkMagic(header.ddspf.fourCC, "DXT5")) {
					m_format = TextureFormat::BC3;
				} else {
					LOG_ERROR("未対応のDDSフォーマット[fourCC={}, path={}]", magic2, path);
					return;
				}
			} else if(header.ddspf.flags & (u32)(DDPF_RGB | DDPF_ALPHAPIXELS | DDPF_ALPHA | DDPF_BUMPDUDV | DDPF_LUMINANCE)) {
				// Bitmask による定義
				unsigned int  bit_count = header.ddspf.RGBBitCount;
				unsigned int  r_mask = header.ddspf.RBitMask;
				unsigned int  g_mask = header.ddspf.GBitMask;
				unsigned int  b_mask = header.ddspf.BBitMask;
				unsigned int  a_mask = header.ddspf.ABitMask;
			} else {
				LOG_ERROR("不正なDDSファイル[fourCC={}, path={}]", TC("aa"), path);
				return;
			}

			if (!(header.caps& DDSCAPS_MIPMAP)) {
				mipLevels = 1;
			}
			if (!(header.caps2& DDSCAPS2_VOLUME)) {
				depth = 1;
			}
			if (header.caps2& DDSCAPS2_CUBEMAP) {
				faceCount = 6;
			}


			if (13384u < width || 13384u < height || 2048u < depth || 15u < mipLevels || 2048u < arraySize) {
				LOG_ERROR("サポート外");
				return;
			}

			// ピクセルバイナリ読み込み
			auto size = fs.size() - fs.position();
			m_blob.resize(size);
			reader.read(m_blob.data(), size);

			s32 index=0;
			for (s32 item = 0; item < arraySize; ++item) {
				for (s32 level = 0; level < mipLevels; ++level, ++index) {

				}
			}

		} else {
			return;
		}

		if (isDDS) {
		} else {

			m_format = TextureFormat::RGBA8;

			byte* pixels = nullptr;
			int width, height, bpp;
			auto p = path.generic_string();
			pixels = stbi_load(p.c_str(), &width, &height, &bpp, 0);
			if (pixels != nullptr) {
				m_blob.append(pixels, (size_t)width * height * bpp);
			}
		}

	}


}// namespace ob
#endif
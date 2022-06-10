//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/TextureData.h>
#include <Framework/Graphic/Private/DDSFormat.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ob::graphic
{

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------

	TextureData::TextureData(const Path& path) 
	{
		if (path.extension() == ".dds") {

		} else {

			byte* pixels = nullptr;
			int width, height, bpp;
			auto p = path.generic_string();
			pixels = stbi_load(p.c_str(), &width, &height, &bpp, 0);
			if (pixels != nullptr) {
				data.append(pixels, (size_t)width * height * bpp);
			}
		}
	}


	template<size_t N, typename T = unsigned int>
	constexpr T getMagic(const char(&code)[N]) {
		T result = 0;
		for (size_t i = 0; i < N - 1; ++i) {
			result = (result << 8) | code[i];
		}
		return result;
	}

	TextureData::TextureData(const void* data, size_t size)
	{

		if (size < sizeof(DDS_HEADER))goto ERROR_END;

		s32 offset = 0;
		auto& header = *GetOffsetPtr<DDS_HEADER>(data, offset);
		offset += sizeof(DDS_HEADER);

		if (header.magic != getMagic(" DDS"))goto ERROR_END;
		if (header.size != sizeof(DDS_HEADER) - sizeof(u32))goto ERROR_END;

		auto& exHeader = *GetOffsetPtr<DDS_HEADER_DXT10>(data, offset);
		bool hasExHeader = (!(header.ddspf.flags & 4)) && (header.ddspf.fourCC == getMagic("DX10"));
		if (hasExHeader) {
			offset += sizeof(DDS_HEADER_DXT10);
		}


		auto width = header.width;
		auto height = header.height;
		auto depth = header.depth;
		auto mipLevels = std::max<u32>(1,header.mipMapCount);
		
		if (hasExHeader) {

		} else {

		}


		// m_subResources.resize(arrayNum*sliceNum*miLevels)

	ERROR_END:

		return;
	}

}// namespace ob
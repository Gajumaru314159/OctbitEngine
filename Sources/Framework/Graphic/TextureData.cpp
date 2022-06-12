//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/TextureData.h>
#include <Framework/Graphic/Private/DDSFormat.h>
#include <Framework/Core/File/BinaryReader.h>
#include <Framework/Core/File/FileStream.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ob::graphic
{

	template<size_t N, typename T = unsigned int>
	constexpr T checkMagic(const byte(&bytes)[N], const char(&code)[N + 1]) {
		for (size_t i = 0; i < N - 1; ++i) {
			if (code[i] != bytes[i])return false;
		}
		return true;
	}



	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------

	TextureData::TextureData(const Path& path)
		: m_size(0, 0)
	{
		FileStream fs(path);
		bool isDDS = false;
		if (fs) {
			BinaryReader reader(fs);

			// ヘッダ読み込み
			DDS_HEADER header;
			if (!reader.read(header))return;
			if (!checkMagic(header.magic, "DDS "))return;

			// 拡張ヘッダ読み込み
			if (header.ddspf.flags & DDPF_FOURCC) {
				DDS_HEADER_DXT10 exHeader;
				if (!reader.read(exHeader))return;
				if (!checkMagic(header.ddspf.fourCC, "DX10"))return;
			}

			auto width = header.width;
			auto height = header.height;
			auto depth = header.depth;
			auto mipLevels = std::max<u32>(1, header.mipMapCount);

		} else {
			return;
		}

		if (isDDS) {
			auto size = fs.size();
			m_blob.resize(size);
			fs.seek(0, SeekOrigin::Begin);
			fs.read(m_blob.data(), size);
			//initializeDDS(m_blob.data(), m_blob.size());
		} else {
			byte* pixels = nullptr;
			int width, height, bpp;
			auto p = path.generic_string();
			pixels = stbi_load(p.c_str(), &width, &height, &bpp, 0);
			if (pixels != nullptr) {
				m_blob.append(pixels, (size_t)width * height * bpp);
			}
		}

	}

	TextureData::TextureData(const void* data, size_t size)
	{
		//initializeDDS(data, size);
	}


}// namespace ob
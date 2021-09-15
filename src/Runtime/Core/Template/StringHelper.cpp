//***********************************************************
//! @file
//! @brief 文字列ヘルパー関数群
//! @author Gajumaru
//***********************************************************
#include <Core/Template/StringHelper.hpp>

#include <uchar.h>

namespace ob
{

	//-----------------------------------------------------------
	//! @brief ワイド文字列をマルチバイト文字列に変換する
	//! @retval TRUE  成功
	//! @retval FALSE 失敗
	//-----------------------------------------------------------
	bool StringHelper::ConvertMultiByte(char* dest, s32 destSize, const char16_t* src, s32 srcSize)
	{
		s32 destPos = 0;

		const s32 len = 6;
		char tmp[len];
		mbstate_t m{};
		bool success = true;
		for (s32 srcIdx = 0; srcIdx < srcSize; ++srcIdx)
		{
			size_t sz = c16rtomb(tmp, *src, &m);

			if ((size_t)destSize - 1 <= destPos + sz || sz == (size_t)-1)
			{
				success = false;
				break;
			}

			for (s32 i = 0; i < (s32)sz; i++)
			{
				*dest = tmp[i];
				++dest;
				destPos++;
			}
		}
		*dest = u'\0';
		return success;
	}

}// namespace ob
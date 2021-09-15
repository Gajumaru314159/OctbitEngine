//***********************************************************
//! @file
//! @brief 文字列ヘルパー関数群
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

namespace ob
{
	//-----------------------------------------------------------
	//! @brief 文字列に関するヘルパー関数群
	//-----------------------------------------------------------
	class StringHelper
	{
	public:

		static bool ConvertMultiByte(char* dest, s32 destSize, const char16_t* src, s32 srcSize);	// ワイド文字列をマルチバイト文字列に変換する

	};
}// namespace ob
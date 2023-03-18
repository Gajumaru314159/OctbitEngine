//***********************************************************
//! @file
//! @brief		ImGui便利関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include "Library/imgui.h"

namespace ImGui {
	inline namespace ob {

		using namespace ::ob;

		inline ImVec4 ToImCol(const Color& col) {
			ImVec4 imcol(col.r, col.g, col.b, col.a);
			return imcol;
		}

	}
}
//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <nlohmann/json.hpp>
#include <fstream>

namespace ob::editor{

	class ReflectionWriter {
	public:
		void output(StringView path);
	};

}

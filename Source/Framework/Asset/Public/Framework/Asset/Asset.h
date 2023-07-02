//***********************************************************
//! @file
//! @brief		アセットアイテム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>

namespace ob::asset {

	class Asset : public RefObject {
	public:

		const String& getFilePath();

	};


}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	//! @brief FrameGraphで使用されるリソースの種類
	enum class FGResourceType : s32 {
		Invalid,
		Texture,
		Buffer,
		Dummy,
	};

	//! @brief フレームグラフリソースを表す構造体
	struct FGResource {
		FGResourceType type = FGResourceType::Invalid;
		s32 value = -1;

		operator bool()const {
			return type != FGResourceType::Invalid;
		}
	};

}
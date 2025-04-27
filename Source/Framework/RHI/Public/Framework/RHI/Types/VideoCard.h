//***********************************************************
//! @file
//! @brief		トポロジー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/MemorySize.h>
#include <Framework/RHI/Types/TextureFormat.h>

namespace ob::rhi {

	struct VideoOutputMode {
		u32				width;		//!< 幅
		u32				height;		//!< 高さ
		f32				refreshRate;//!< リフレッシュレート
		TextureFormat	format;		//!< フォーマット
	};

	struct VideoOutput {
		String	name;		//!< 名前
		IntRect	rect;		//!< 出力領域
		bool	isPrimary;  //!< プライマリフラグ
		VideoOutputMode			currentMode;	//!< 現在のモード
		Vector<VideoOutputMode> modes;			//!< モードリスト
	};

	struct VideoCard {
		String				name;		//!< 名前
		MemorySize			memory;		//!< メモリサイズ
		u32					deviceId;	//!< デバイスID
		Vector<VideoOutput> outputs;	//!< 出力先
	};

}
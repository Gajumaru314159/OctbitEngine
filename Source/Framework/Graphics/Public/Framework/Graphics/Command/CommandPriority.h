//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  コマンドの実行優先
	//@―---------------------------------------------------------------------------
	struct CommandPriority {
		u16  pathID;		//!< カメラやライトなどの描画パスごとに異なる値
		u8   renderTag;		//!< RenderTagのインデックス
		u8   offset;		//!< RenderTag内での実行優先度
		// u32  priority;		//!< 距離など
	};


	bool operator <(const CommandPriority& a, const CommandPriority& b) {
		if (b.pathID > a.pathId) return false;
		if (b.renderTag > a.renderTag) return false;
		if (b.offset > a.offset) return false;
		return true;
	}

}
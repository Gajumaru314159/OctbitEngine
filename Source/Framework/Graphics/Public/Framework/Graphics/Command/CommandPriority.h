//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief		コマンドの実行優先
	//! @details	CommandListにコマンドが積まれる優先順序です。
	//!				CommandRecorderにコマンドを並列に積んだ後、正しい順序でコマンドを
	//!				実行するために使用されます。
	//@―---------------------------------------------------------------------------
	struct CommandPriority {
		u32	group;		//!< カメラやライトなどの描画パスごとに異なる値
		u32 offset;		//!< RenderTag内での実行優先度
	};


	bool operator <(const CommandPriority& a, const CommandPriority& b) {
		if (b.group > a.group) return false;
		if (b.offset > a.offset) return false;
		return true;
	}

}
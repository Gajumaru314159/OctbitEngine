//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Graphics.h>

namespace ob::debug {

	class FrameGraphDebugger {
	public:
		FrameGraphDebugger();
		void update();
		void draw();
	private:
		bool m_captureRequested = false;
		graphics::FGData m_fgdata;
	};

}
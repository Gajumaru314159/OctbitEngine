//***********************************************************
//! @file
//! @brief		マウス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Log/Logger.h>

namespace ob::debug {


	class LogInfo {
	public:
		LogInfo();
		void update();
	private:

		struct LogCache {
			DateTime datetime;
			LogLevel level;
			String message;
			String file;
			String line;
		};

		bool m_bAutoWrap = false;
		Logger::EventHandle m_hLogged;
		Deque<LogCache>	m_logs;
		size_t		m_maxLogCount;
		HashMap<LogLevel, bool> m_levelFilter;
		HashMap<LogLevel, Color> m_levelColors;

	};

}
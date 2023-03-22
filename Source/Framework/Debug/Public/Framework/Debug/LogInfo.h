//***********************************************************
//! @file
//! @brief		マウス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Log/Logger.h>
#include <Plugins/ImGui/ImGui.h>

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
		HashMap<LogLevel, String> m_levelNames;
		ImGuiTextFilter m_filter;

	};

}
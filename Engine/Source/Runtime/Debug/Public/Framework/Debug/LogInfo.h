//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Thread/SpinLock.h>
#include <Framework/Core/Log/Logger.h>
#include <Framework/ImGui/ImGui.h>

namespace ob::debug {


	class LogInfo {
	public:
		LogInfo();
		void draw();
	private:

		struct LogCache {
			DateTime datetime;
			LogLevel level;
			String message;
			String file;
			String path;
			String line;
			s32 line2;
			s32 count;
		};

		bool m_bAutoWrap = false;
		Logger::EventHandle m_hLogged;
		SpinLock		m_lock;
		Deque<LogCache>	m_logs;
		size_t		m_maxLogCount;
		HashMap<LogLevel, bool> m_levelFilter;
		HashMap<LogLevel, Color> m_levelColors;
		HashMap<LogLevel, String> m_levelNames;
		ImGuiTextFilter m_filter;

	};

}
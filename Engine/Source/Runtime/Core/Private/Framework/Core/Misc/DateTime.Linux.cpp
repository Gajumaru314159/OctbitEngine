//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/DateTime.h>
#include <chrono>

#ifdef OS_LINUX

namespace ob::core {

	//!	@brief		現在の日時を取得
	DateTime DateTime::Now() {

		DateTime dt{};

		std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
		time_t now = time(NULL);

		if (tm* p = gmtime(&now)) {
			dt.year = p->tm_year + 1900;
			dt.month = p->tm_mon + 1;
			dt.day = p->tm_mday;
			dt.hour = p->tm_hour;
			dt.minute = p->tm_min;
			dt.second = p->tm_sec;
		} else {
			LOG_ERROR_EX("Core", "現在時刻の取得に失敗。");
		}
		auto msec = duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
		dt.milliSecond = std::chrono::milliseconds(msec).count() % 1000;

		return dt;
	}

}

#endif
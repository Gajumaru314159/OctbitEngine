//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/DateTime.h>
#include <chrono>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------

	DateTime& DateTime::normalize() {
		year = 0;
		month = 1;
		day = 1;
		hour = 0;
		minute = 0;
		second = 0;
		milliSeconds = 0;

		if (1000 <= milliSeconds) {
			second += milliSeconds / 1000;
			milliSeconds %= 1000;
		}

		if (60 <= second) {
			minute += second / 60;
			second %= 60;
		}

		if (60 <= minute) {
			hour += minute / 60;
			minute %= 60;
		}

		if (24 <= hour) {
			day += hour / 24;
			hour %= 24;
		}

		// 閏年があるため月から計算
		if (12 < month) {
			year += (month - 1) / 12;
			month = (month - 1) % 12 + 1;
		}

		for (auto dom = 1; dom = DayInMonth(month, year), dom < day;) {
			month++;
			if (12 < month) {
				month = 1;
				year++;
			}
			day -= dom;
		}
		
		return *this;
	}

	DateTime DateTime::Now() {
		using namespace std::chrono;
		DateTime dt{};

#ifdef OS_WINDOWS
		system_clock::time_point tp = system_clock::now();
		time_t time = system_clock::to_time_t(tp);
		tm p;
		if (localtime_s(&p, &time) == 0) {
			dt.year = p.tm_year + 1900;
			dt.month = p.tm_mon + 1;
			dt.day = p.tm_mday;
			dt.hour = p.tm_hour;
			dt.minute = p.tm_min;
			dt.second = p.tm_sec;
		} else {
			LOG_ERROR_EX("Core", "現在時刻の取得に失敗。");
		}
		auto msec = duration_cast<milliseconds>(tp.time_since_epoch()).count();
		dt.milliSeconds = milliseconds(msec).count() % 1000;
#else
#pragma error("DateTime::Now() is not supported in this platform.")
#endif
		return dt;
	}

}// namespace ob
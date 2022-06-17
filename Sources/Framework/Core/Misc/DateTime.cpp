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
		//TODO DataTimeの正規化
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
		static_assert("Duration::Now() is not implemented in this platform.");
#endif
		return dt;
	}

}// namespace ob
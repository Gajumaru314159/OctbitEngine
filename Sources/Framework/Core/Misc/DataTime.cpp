//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/DataTime.h>
#include <chrono>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------

	DataTime& DataTime::normalize() {
		//TODO DataTimeの正規化
		return *this;
	}

	DataTime DataTime::Now() {
		using namespace std::chrono;
		DataTime dt{};

#ifdef OS_WINDOWS
		system_clock::time_point tp = system_clock::now();
		auto msec = duration_cast<milliseconds>(tp.time_since_epoch()).count() % 1000;
		time_t time = system_clock::to_time_t(tp);
		tm p;
		if (localtime_s(&p, &time)) {
			dt.year			= p.tm_year;
			dt.month		= p.tm_mon+1;
			dt.day			= p.tm_mday;
			dt.hour			= p.tm_hour;
			dt.minute		= p.tm_min;
			dt.second		= p.tm_sec;
		}else{
			LOG_ERROR_EX("Core", "現在時刻の取得に失敗。");
		}
		dt.milliSeconds = milliseconds(msec).count() %1000;
#else
		static_assert("TimeSpan::Now() is not implemented in this platform.");
#endif
		return dt;
	}

}// namespace ob
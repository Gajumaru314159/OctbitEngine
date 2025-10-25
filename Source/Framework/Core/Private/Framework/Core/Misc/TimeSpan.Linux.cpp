//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/TimeSpan.h>

#ifdef OS_LINUX
#include <ctime>

namespace ob::core {

	//! @brief  システムを起動してからの時間
	TimeSpan TimeSpan::FromSystemLaunch() {
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		auto tickCount = static_cast<uint64_t>(ts.tv_sec) * 1000ULL + static_cast<uint64_t>(ts.tv_nsec) / 1000000ULL;
		return TimeSpan::MilliSeconds(gsl::narrow_cast<f64>(tickCount));
	}

}
#endif
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/TimeSpan.h>

#ifdef OS_WINDOWS
#include <Windows.h>

namespace ob::core {

	//! @brief  システムを起動してからの時間
	TimeSpan TimeSpan::FromSystemLaunch() {
		return TimeSpan::MilliSeconds(gsl::narrow_cast<f64>(GetTickCount64()));
	}

}

#endif
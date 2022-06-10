﻿//***********************************************************
//! @file
//! @brief		時間間隔
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/TimeSpan.h>

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  システムを起動してからの時間
	//@―---------------------------------------------------------------------------
	TimeSpan::TimeSpan(const DateTime& from, const DateTime& to) noexcept {

		auto getDays = [](const DateTime& dt) {
			return dt.year * 365 + (dt.year/4) - (dt.year / 100) + (dt.year / 400);
		};

		*this = 			
		TimeSpan(
			getDays(to) - getDays(from),
			to.hour - from.hour,
			to.minute - from.minute,
			to.second - from.second,
			to.milliSeconds - from.milliSeconds
		);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  システムを起動してからの時間
	//@―---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromSystemLaunch() {
#ifdef OS_WINDOWS
		return TimeSpan::MilliSeconds(GetTickCount());
#else
		static_assert("TimeSpan::FromSystemLaunch() is not implemented in this platform.");
#endif
	}


	//@―---------------------------------------------------------------------------
	//! @brief  アプリを起動してからの時間
	//@―---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromAppLaunch() {
#ifdef OS_WINDOWS
		//TODO アプリ起動からの時間
		return TimeSpan::MilliSeconds(0);
#else
		static_assert("TimeSpan::FromSystemLaunch() is not implemented in this platform.");
#endif
	}

}// namespace ob
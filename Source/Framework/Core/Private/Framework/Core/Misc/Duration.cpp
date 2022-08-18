//***********************************************************
//! @file
//! @brief		時間間隔
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Duration.h>

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  システムを起動してからの時間
	//@―---------------------------------------------------------------------------
	Duration::Duration(const DateTime& from, const DateTime& to) noexcept {

		auto getDays = [](const DateTime& dt) {
			return dt.year * 365 + (dt.year/4) - (dt.year / 100) + (dt.year / 400);
		};

		*this = 			
		Duration(
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
	Duration Duration::FromSystemLaunch() {
#ifdef OS_WINDOWS
		return Duration::MilliSeconds(GetTickCount64());
#else
		static_assert("Duration::FromSystemLaunch() is not implemented in this platform.");
#endif
	}


	//@―---------------------------------------------------------------------------
	//! @brief  アプリを起動してからの時間
	//@―---------------------------------------------------------------------------
	Duration Duration::FromAppLaunch() {
#ifdef OS_WINDOWS
		//TODO アプリ起動からの時間
		return FromSystemLaunch();
#else
		static_assert("Duration::FromSystemLaunch() is not implemented in this platform.");
#endif
	}

}// namespace ob
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/TimeSpan.h>

namespace ob::core {

	//! @brief  システムを起動してからの時間
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
			to.milliSecond - from.milliSecond
		);
	}

}
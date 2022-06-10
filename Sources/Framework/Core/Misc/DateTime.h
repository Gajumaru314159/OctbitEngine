//***********************************************************
//! @file
//! @brief		日付と時刻
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  曜日
	//@―---------------------------------------------------------------------------
	enum class DayOfWeek {
		Sunday,     //!< 日曜日
		Monday,     //!< 月曜日 
		Tuesday,    //!< 火曜日
		Wednesday,  //!< 水曜日
		Thursday,   //!< 木曜日
		Friday,     //!< 金曜日
		Saturday,   //!< 土曜日
	};


	//@―---------------------------------------------------------------------------
	//! @brief		日時型
	//! 
	//! @details	各種関数は時刻が正規化されているものとして計算されます。
	//@―---------------------------------------------------------------------------
	struct DateTime {
	public:
		s32			year = 0;						//!< 年
		s32			month = 1;						//!< 月
		s32			day = 1;						//!< 日
		s32			hour = 0;						//!< 時間
		s32			minute = 0;						//!< 分
		s32			second = 0;						//!< 秒
		s32			milliSeconds = 0;				//!< ミリ秒
	public:
		DateTime& normalize();
		bool isMorning()const;
		bool isAfternoon()const;
		s32 daysInYear();
	public:
		static bool IsLerpYear(s32 year);
	public:
		static s32 DayInMonth(s32 month, s32 year);

		static DateTime Now();						//!< システムの現在時刻を取得
	};


	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//!	@brief	午前か
	//@―---------------------------------------------------------------------------
	inline bool DateTime::isMorning()const {
		return DateTime(*this).normalize().hour < 12;
	}

	//@―---------------------------------------------------------------------------
	//!	@brief	午後か
	//@―---------------------------------------------------------------------------
	inline bool DateTime::isAfternoon()const {
		return !isMorning();
	}

	//@―---------------------------------------------------------------------------
	//!	@brief	月の日数を計算
	//@―---------------------------------------------------------------------------
	inline s32 DateTime::DayInMonth(s32  month, s32 year) {
		switch (month) {
		case 2:
			return IsLerpYear(year) ? 29 : 28;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		default:
			return 0;
		}
	}

	//@―---------------------------------------------------------------------------
	//!	@brief	閏年か
	//@―---------------------------------------------------------------------------
	inline bool DateTime::IsLerpYear(s32 year) {
		return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
	}

	//@―---------------------------------------------------------------------------
	//!	@brief	1月1日から数えて何日目か
	//@―---------------------------------------------------------------------------
	inline s32 DateTime::daysInYear() {
		s32 sum = day;
		for (s32 m = 1; m < month; ++m) {
			sum += DayInMonth(m, year);
		}
		return sum;
	}


	//! @endcond
}// namespcae ob


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::DateTime, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const ob::core::DateTime& dt, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), TC("{:>4}-{:0>2}-{:0>2}T{:0>2}:{:0>2}:{:0>2},{:0>2}"), dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.milliSeconds / 10);
	}
};
//! @endcond
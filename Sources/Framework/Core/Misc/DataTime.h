﻿//***********************************************************
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
	struct DataTime {
	public:
		s32			year = 0;						//!< 年
		s32			month = 1;						//!< 月
		s32			day = 1;						//!< 日
		s32			hour = 0;						//!< 時間
		s32			minute = 0;						//!< 分
		s32			second = 0;						//!< 秒
		s32			milliSeconds = 0;				//!< ミリ秒
	public:
		DataTime& normalize();
		bool isMorning()const;
		bool isAfternoon()const;
		s32 daysInYear();
	public:
		static bool IsLerpYear(s32 year);
	public:
		static s32 DayInMonth(s32 month, s32 year);

		static DataTime Now();						//!< システムの現在時刻を取得
	};


	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	bool DataTime::isMorning()const {
		return DataTime(*this).normalize().hour < 12;
	}
	bool DataTime::isAfternoon()const {
		return !isMorning();
	}

	s32 DataTime::DayInMonth(s32  month, s32 year) {
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

	bool DataTime::IsLerpYear(s32 year) {
		return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
	}


	s32 DataTime::daysInYear() {
		s32 sum = day;
		for (s32 m = 1; m < month; ++m) {
			sum += DayInMonth(m, year);
		}
		return sum;
	}


	//! @endcond
}// namespcae ob
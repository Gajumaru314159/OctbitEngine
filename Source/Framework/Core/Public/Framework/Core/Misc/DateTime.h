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
		Sunday = 0, //!< 日曜日
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

		bool operator==(const DateTime& rhs)const noexcept;			//!< 等価演算子
		bool operator!=(const DateTime& rhs)const noexcept;			//!< 否等価演算子
		bool operator<(const DateTime& rhs)const noexcept;			//!< 比較演算子
		bool operator<=(const DateTime& rhs)const noexcept;			//!< 比較演算子
		bool operator>(const DateTime& rhs)const noexcept;			//!< 比較演算子
		bool operator>=(const DateTime& rhs)const noexcept;			//!< 比較演算子

		//@―---------------------------------------------------------------------------
		//!	@brief	正規化
		//@―---------------------------------------------------------------------------
		DateTime& normalize();

		//@―---------------------------------------------------------------------------
		//!	@brief	午前か
		//@―---------------------------------------------------------------------------
		bool isMorning()const;

		//@―---------------------------------------------------------------------------
		//!	@brief	午後か
		//@―---------------------------------------------------------------------------
		bool isAfternoon()const;

		//@―---------------------------------------------------------------------------
		//!	@brief	曜日
		//@―---------------------------------------------------------------------------
		DayOfWeek dayOfWeek()const;

		//@―---------------------------------------------------------------------------
		//!	@brief		1月1日から数えて何日目か
		//! 
		//! @details	1月1日を1として何日目かを返す。
		//!				閏年の場合3月以降の日数がずれる。
		//@―---------------------------------------------------------------------------
		s32 daysInYear()const;

		//@―---------------------------------------------------------------------------
		//!	@brief		日時を文字列に変換
		//! 
		//! @details	使用可能なフォーマットは以下の通りです。
		//!				必要に応じてエスケープ文字を使用してください。
		//!				* yyyyy	年 (5 桁の数値)
		//!				* yyyy	年 (4 桁の数値)
		//!				* yy	年 (00 ～ 99)
		//!				* y		年 ( 0 ～ 99)
		//!				* MMMM	英語の月名 (January-Febuarty)
		//!				* MMM	英語の月名の略称 (Jan-Feb)
		//!				* MM	月 (01 ～ 12)
		//!				* M		月 ( 1 ～ 12)
		//!				* dddd	英語の曜日 (Sunday-Saturday)
		//!				* ddd	英語の曜日の略称 (Sun-Sat)
		//!				* dd	月の日にち (1 ～ 31)
		//!				* d		月の日にち (01 ～ 31)
		//!				* tt	AM / PM
		//!				* t		A / P
		//!				* HH	24 時間形式の時間 (00 ～ 23)
		//!				* H		24 時間形式の時間 (0 ～ 23)
		//!				* hh	12 時間形式の時間 (01 ～ 12)
		//!				* h		12 時間形式の時間 (1 ～ 12)
		//!				* mm	分 (00 ～ 59)
		//!				* m		分 ( 0 ～ 59)
		//!				* ss	秒 (00 ～ 59)
		//!				* s		秒 ( 0 ～ 59)
		//!				* fff	小数点以下3桁の秒
		//!				* ff	小数点以下2桁の秒
		//!				* f		小数点以下1桁の秒
		//! @ref		https://learn.microsoft.com/ja-jp/dotnet/standard/base-types/custom-date-and-time-format-strings
		//@―---------------------------------------------------------------------------
		String toString(StringView format = TC("yyyy-MM-dd HH:mm:ss.ff"))const;

	public:

		//@―---------------------------------------------------------------------------
		//!	@brief	閏年か
		//@―---------------------------------------------------------------------------
		static bool IsLerpYear(s32 year);

	public:

		//@―---------------------------------------------------------------------------
		//!	@brief	月の日数を計算
		//@―---------------------------------------------------------------------------
		static s32 DayInMonth(s32 month, s32 year);

		//@―---------------------------------------------------------------------------
		//! [brief	システムの現在時刻を取得
		//@―---------------------------------------------------------------------------
		static DateTime Now();	

	};


	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//===============================================================
	// 比較
	//===============================================================
	inline bool DateTime::operator==(const DateTime& rhs)const noexcept {
		return
			year == rhs.year &&
			month == rhs.month &&
			day == rhs.day &&
			hour == rhs.hour &&
			minute == rhs.minute &&
			second == rhs.second &&
			milliSeconds == rhs.milliSeconds;
	}
	inline bool DateTime::operator!=(const DateTime& rhs)const noexcept {
		return !(*this == rhs);
	}
	inline bool DateTime::operator<(const DateTime& rhs)const noexcept {
		auto l = *this; l.normalize();
		auto r = rhs; r.normalize();

		if (year < r.year)return true;
		if (month < r.month)return true;
		if (day < r.day)return true;
		if (hour < r.hour)return true;
		if (minute < r.minute)return true;
		if (second < r.second)return true;
		if (milliSeconds < r.milliSeconds)return true;

		return false;
	}
	inline bool DateTime::operator<=(const DateTime& rhs)const noexcept {
		auto l = *this; l.normalize();
		auto r = rhs; r.normalize();

		if (year <= r.year)return true;
		if (month <= r.month)return true;
		if (day <= r.day)return true;
		if (hour <= r.hour)return true;
		if (minute <= r.minute)return true;
		if (second <= r.second)return true;
		if (milliSeconds <= r.milliSeconds)return true;

		return false;
	}
	inline bool DateTime::operator>(const DateTime& rhs)const noexcept {
		return !(*this <= rhs);
	}
	inline bool DateTime::operator>=(const DateTime& rhs)const noexcept {
		return !(*this < rhs);
	}


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
		if (12 < month) {
			year += (month + 11) / 12;
			month = (month + 11) % 12 + 1;
		}


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
	inline s32 DateTime::daysInYear()const {
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
		return format_to(ctx.out(), dt.toString().c_str());
	}
};
//! @endcond
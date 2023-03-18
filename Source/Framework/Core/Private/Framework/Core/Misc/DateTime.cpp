//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/DateTime.h>
#include <chrono>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  日時の正規化
	//@―---------------------------------------------------------------------------
	DateTime& DateTime::normalize() {
		year = 0;
		month = 1;
		day = 1;
		hour = 0;
		minute = 0;
		second = 0;
		milliSeconds = 0;

		if (1000 <= milliSeconds) {
			second += milliSeconds / 1000;
			milliSeconds %= 1000;
		}

		if (60 <= second) {
			minute += second / 60;
			second %= 60;
		}

		if (60 <= minute) {
			hour += minute / 60;
			minute %= 60;
		}

		if (24 <= hour) {
			day += hour / 24;
			hour %= 24;
		}

		// 閏年があるため月から計算
		if (12 < month) {
			year += (month - 1) / 12;
			month = (month - 1) % 12 + 1;
		}

		for (auto dom = 1; dom = DayInMonth(month, year), dom < day;) {
			month++;
			if (12 < month) {
				month = 1;
				year++;
			}
			day -= dom;
		}

		return *this;
	}

	//@―---------------------------------------------------------------------------
	//!	@brief	曜日
	//@―---------------------------------------------------------------------------
	DayOfWeek DateTime::dayOfWeek()const {
		auto k = year % 100;
		auto j = year / 100;
		auto num = (day + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 + 5 * j + 6) % 7;
		return static_cast<DayOfWeek>(num);
	}

	//@―---------------------------------------------------------------------------
	//!	@brief		日時を文字列に変換
	//@―---------------------------------------------------------------------------
	String DateTime::toString(StringView format)const {

		static constexpr StringView MONTH_TEXT[]
		{
			TC("January"),
			TC("February"),
			TC("March"),
			TC("April"),
			TC("May"),
			TC("June"),
			TC("July"),
			TC("August"),
			TC("September"),
			TC("October"),
			TC("November"),
			TC("December"),
		};
		static constexpr StringView MONTH_SHORT_TEXT[]
		{
			TC("Jan"),
			TC("Feb"),
			TC("Mar"),
			TC("Apr"),
			TC("May"),
			TC("Jun"),
			TC("Jul"),
			TC("Aug"),
			TC("Sep"),
			TC("Oct"),
			TC("Nov"),
			TC("Dec"),
		};

		static constexpr StringView DAY_OF_WEEK_TEXT[]
		{
			TC("Sunday"),
			TC("Monday"),
			TC("Tuesday"),
			TC("Wednesday"),
			TC("Thursday"),
			TC("Friday"),
			TC("Saturday"),
		};
		static constexpr StringView DAY_OF_WEEK_SHORT_TEXT[]
		{
			TC("Sun"),
			TC("Mon"),
			TC("Tue"),
			TC("Wed"),
			TC("Thu"),
			TC("Fri"),
			TC("Sat"),
		};


		size_t i = 0;

		auto read = [&format, &i](StringView pattern) {
			if (format.substr(i).starts_with(pattern)) {
				i += pattern.size();
				return true;
			}
			return false;
		};

		String out;
		for (; i < format.size();) {

			if (format[i] == TC('\\')) {
				++i;
				if (i < format.size()) {
					out.append(1,format[i++]);
				}
				continue;
			}

			if (false);
			else if (read("yyyyy"))out += Format(TC("{:05}"), year);
			else if (read("yyyy"))out += Format(TC("{:04}"), year);
			else if (read("yy"))out += Format(TC("{:02}"), year % 100);
			else if (read("y"))out += Format(TC("{}"), year % 100);
			else if (read("MMMM"))out += MONTH_TEXT[((u32)month + 11) % 12];
			else if (read("MMM"))out += MONTH_SHORT_TEXT[((u32)month + 11) % 12];
			else if (read("MM"))out += Format(TC("{:02}"), month);
			else if (read("M"))out += Format(TC("{:}"), month);
			else if (read("dddd"))out += DAY_OF_WEEK_TEXT[((u32)enum_cast(dayOfWeek())) % 7];
			else if (read("ddd"))out += DAY_OF_WEEK_SHORT_TEXT[((u32)enum_cast(dayOfWeek())) % 7];
			else if (read("dd"))out += Format(TC("{:02}"), day);
			else if (read("d"))out += Format(TC("{:}"), day);
			else if (read("tt"))out += isMorning() ? TC("AM") : TC("PM");
			else if (read("t"))out += isMorning() ? TC("A") : TC("P");
			else if (read("HH"))out += Format(TC("{:02}"), hour);
			else if (read("H"))out += Format(TC("{:}"), hour);
			else if (read("hh"))out += Format(TC("{:02}"), (hour + 23) % 12 + 1);
			else if (read("h"))out += Format(TC("{}"), (hour + 23) % 12 + 1);
			else if (read("mm"))out += Format(TC("{:02}"), minute);
			else if (read("m"))out += Format(TC("{}"), minute);
			else if (read("ss"))out += Format(TC("{:02}"), second);
			else if (read("s"))out += Format(TC("{}"), second);
			else if (read("fff"))out += Format(TC("{:03}"), milliSeconds);
			else if (read("ff"))out += Format(TC("{:02}"), milliSeconds / 10);
			else if (read("f"))out += Format(TC("{:01}"), milliSeconds / 100);
			else out.append(1,format[i++]);

		}

		return std::move(out);

	}


	//@―---------------------------------------------------------------------------
	//!	@brief		現在の日時を取得
	//@―---------------------------------------------------------------------------
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
#pragma error("DateTime::Now() is not supported in this platform.")
#endif
		return dt;
	}

}// namespace ob
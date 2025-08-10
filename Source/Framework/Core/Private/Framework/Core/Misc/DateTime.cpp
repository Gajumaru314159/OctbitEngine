//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/DateTime.h>
#include <chrono>

namespace ob::core {

	//!	@brief	曜日
	DayOfWeek DateTime::dayOfWeek()const {
		auto k = year % 100;
		auto j = year / 100;
		auto num = (day + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 + 5 * j + 6) % 7;
		return static_cast<DayOfWeek>(num);
	}

	//!	@brief		日時を文字列に変換
	String DateTime::toString(StringView format)const {

		static constexpr StringView MONTH_TEXT[]
		{
			"January",
			"February",
			"March",
			"April",
			"May",
			"June",
			"July",
			"August",
			"September",
			"October",
			"November",
			"December",
		};
		static constexpr StringView MONTH_SHORT_TEXT[]
		{
			"Jan",
			"Feb",
			"Mar",
			"Apr",
			"May",
			"Jun",
			"Jul",
			"Aug",
			"Sep",
			"Oct",
			"Nov",
			"Dec",
		};

		static constexpr StringView DAY_OF_WEEK_TEXT[]
		{
			"Sunday",
			"Monday",
			"Tuesday",
			"Wednesday",
			"Thursday",
			"Friday",
			"Saturday",
		};
		static constexpr StringView DAY_OF_WEEK_SHORT_TEXT[]
		{
			"Sun",
			"Mon",
			"Tue",
			"Wed",
			"Thu",
			"Fri",
			"Sat",
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

			if (format[i] == '\\') {
				++i;
				if (i < format.size()) {
					out.append(1,format[i++]);
				}
				continue;
			}

			/**/ if (read("yyyyy"))out += Format("{:05}", year);
			else if (read("yyyy"))out += Format("{:04}", year);
			else if (read("yy"))out += Format("{:02}", year % 100);
			else if (read("y"))out += Format("{}", year % 100);
			else if (read("MMMM"))out += MONTH_TEXT[(static_cast<u32>(month) + 11) % 12];
			else if (read("MMM"))out += MONTH_SHORT_TEXT[(static_cast<u32>(month) + 11) % 12];
			else if (read("MM"))out += Format("{:02}", month);
			else if (read("M"))out += Format("{:}", month);
			else if (read("dddd"))out += DAY_OF_WEEK_TEXT[static_cast<u32>(enum_cast(dayOfWeek())) % 7];
			else if (read("ddd"))out += DAY_OF_WEEK_SHORT_TEXT[static_cast<u32>(enum_cast(dayOfWeek())) % 7];
			else if (read("dd"))out += Format("{:02}", day);
			else if (read("d"))out += Format("{:}", day);
			else if (read("tt"))out += isMorning() ? "AM" : "PM";
			else if (read("t"))out += isMorning() ? "A" : "P";
			else if (read("HH"))out += Format("{:02}", hour);
			else if (read("H"))out += Format("{:}", hour);
			else if (read("hh"))out += Format("{:02}", (hour + 23) % 12 + 1);
			else if (read("h"))out += Format("{}", (hour + 23) % 12 + 1);
			else if (read("mm"))out += Format("{:02}", minute);
			else if (read("m"))out += Format("{}", minute);
			else if (read("ss"))out += Format("{:02}", second);
			else if (read("s"))out += Format("{}", second);
			else if (read("fff"))out += Format("{:03}", milliSecond);
			else if (read("ff"))out += Format("{:02}", milliSecond / 10);
			else if (read("f"))out += Format("{:01}", milliSecond / 100);
			else out.append(1,format[i++]);

		}

		return out;

	}


	//!	@brief		現在の日時を取得
	DateTime DateTime::Now() {

		DateTime dt{};

#ifdef OS_WINDOWS
		std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(tp);
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
		auto msec = duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
		dt.milliSecond = std::chrono::milliseconds(msec).count() % 1000;
#else
#pragma error("DateTime::Now() is not supported in this platform.")
#endif
		return dt;
	}

}
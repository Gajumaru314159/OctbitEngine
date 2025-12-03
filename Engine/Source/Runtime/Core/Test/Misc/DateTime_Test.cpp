//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/DateTime.h>

using namespace ob;

TEST(DateTime, Constructor)
{
	DateTime dt{};
	EXPECT_EQ(dt.year, 0);
	EXPECT_EQ(dt.month, 1);
	EXPECT_EQ(dt.day, 1);
	EXPECT_EQ(dt.hour, 0);
	EXPECT_EQ(dt.minute, 0);
	EXPECT_EQ(dt.second, 0);
	EXPECT_EQ(dt.milliSecond, 0);
}

TEST(DateTime, IsLerpYear)
{
	EXPECT_TRUE(DateTime::IsLerpYear(2000));  // 400の倍数
	EXPECT_TRUE(DateTime::IsLerpYear(2004));  // 4の倍数かつ100の倍数でない
	EXPECT_FALSE(DateTime::IsLerpYear(1900)); // 100の倍数だが400の倍数でない
	EXPECT_FALSE(DateTime::IsLerpYear(2001)); // 4の倍数でない
	EXPECT_TRUE(DateTime::IsLerpYear(2024));  // 4の倍数かつ100の倍数でない
}

TEST(DateTime, DayInMonth)
{
	// 31日の月
	EXPECT_EQ(DateTime::DayInMonth(1, 2023), 31);
	EXPECT_EQ(DateTime::DayInMonth(3, 2023), 31);
	EXPECT_EQ(DateTime::DayInMonth(5, 2023), 31);
	EXPECT_EQ(DateTime::DayInMonth(7, 2023), 31);
	EXPECT_EQ(DateTime::DayInMonth(8, 2023), 31);
	EXPECT_EQ(DateTime::DayInMonth(10, 2023), 31);
	EXPECT_EQ(DateTime::DayInMonth(12, 2023), 31);

	// 30日の月
	EXPECT_EQ(DateTime::DayInMonth(4, 2023), 30);
	EXPECT_EQ(DateTime::DayInMonth(6, 2023), 30);
	EXPECT_EQ(DateTime::DayInMonth(9, 2023), 30);
	EXPECT_EQ(DateTime::DayInMonth(11, 2023), 30);

	// 2月（非閏年）
	EXPECT_EQ(DateTime::DayInMonth(2, 2023), 28);

	// 2月（閏年）
	EXPECT_EQ(DateTime::DayInMonth(2, 2024), 29);
}

TEST(DateTime, ComparisonOperators)
{
	DateTime dt1{2023, 6, 15, 12, 30, 45, 500};
	DateTime dt2{2023, 6, 15, 12, 30, 45, 500};
	DateTime dt3{2023, 6, 15, 12, 30, 45, 600};

	// 等価
	EXPECT_TRUE(dt1 == dt2);
	EXPECT_FALSE(dt1 == dt3);
	EXPECT_FALSE(dt1 != dt2);
	EXPECT_TRUE(dt1 != dt3);

	// 比較
	EXPECT_TRUE(dt1 < dt3);
	EXPECT_FALSE(dt3 < dt1);
	EXPECT_TRUE(dt1 <= dt2);
	EXPECT_TRUE(dt1 <= dt3);
	EXPECT_FALSE(dt3 <= dt1);
	EXPECT_TRUE(dt3 > dt1);
	EXPECT_FALSE(dt1 > dt3);
	EXPECT_TRUE(dt2 >= dt1);
	EXPECT_TRUE(dt3 >= dt1);
	EXPECT_FALSE(dt1 >= dt3);
}

TEST(DateTime, UtilityFunctions)
{
	DateTime dt{2023, 6, 15, 8, 30, 45, 500};
	
	// 午前/午後
	EXPECT_TRUE(dt.isMorning());
	EXPECT_FALSE(dt.isAfternoon());

	dt.hour = 14;
	EXPECT_FALSE(dt.isMorning());
	EXPECT_TRUE(dt.isAfternoon());

	dt.hour = 12;
	EXPECT_FALSE(dt.isMorning());
	EXPECT_TRUE(dt.isAfternoon());
}

TEST(DateTime, ToString)
{
	DateTime dt{2023, 6, 15, 14, 30, 45, 123};
	
	// デフォルトフォーマット
	String result = dt.toString();
	EXPECT_EQ(result,"2023-06-15 14:30:45.12");

	// カスタムフォーマット
	EXPECT_EQ(dt.toString("yyyy"), "2023");
	EXPECT_EQ(dt.toString("MM"), "06");
	EXPECT_EQ(dt.toString("dd"), "15");
	EXPECT_EQ(dt.toString("HH"), "14");
	EXPECT_EQ(dt.toString("mm"), "30");
	EXPECT_EQ(dt.toString("ss"), "45");
	EXPECT_EQ(dt.toString("fff"), "123");
}
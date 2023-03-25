///***********************************************************
//! @file
//! @brief		Loggerテスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Misc/DateTime.h>
#include <Framework/Core/Misc/TimeSpan.h>

TEST(UserDefinedFormat, DateTime) {
	auto a = Format(TC("{:.3}"), Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f));
	EXPECT_EQ(Format(TC("{}")	, Vec2(1, 2))									, TC("(1,2)"));
	EXPECT_EQ(Format(TC("{:.3}"), Vec3(1.5235223f, 2.11f, -0.01f))				, TC("(1.52,2.11,-0.01)"));
	EXPECT_EQ(Format(TC("{:.3}"), Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f))	, TC("(3.14,2.24e+06,-0.01,-0.00123)"));
	EXPECT_EQ(Format(TC("{}")	, IntVec2(4, 12))								, TC("(4,12)"));
	EXPECT_EQ(Format(TC("{}")	, IntVec3(474, 124, 12))						, TC("(474,124,12)"));
	EXPECT_EQ(Format(TC("{}")	, IntVec4(45, 1472, 313, 1444))					, TC("(45,1472,313,1444)"));
	EXPECT_EQ(Format(TC("{}")	, Quat::Identity)								, TC("(0,0,0,1)"));
	EXPECT_EQ(Format(TC("{}")	, Rot(180, 90, 45))								, TC("(180,90,45)"));
	EXPECT_EQ(Format(TC("{:.3}"), Rot(180, 90, 45))								, TC("(180,90,45)"));
	EXPECT_EQ(Format(TC("{}")	, TimeSpan::Days(23.12534573))					, TC("23d03h00m"));
	EXPECT_EQ(Format(TC("{}")	, TimeSpan::Hours(23.12534573))					, TC("23h07m31s"));
	EXPECT_EQ(Format(TC("{}")	, TimeSpan::Minutes(23.12534573))				, TC("23m7.5207s"));
	EXPECT_EQ(Format(TC("{}")	, TimeSpan::Seconds(23.12534573))				, TC("23.125s"));
	EXPECT_EQ(Format(TC("{}")	, TimeSpan::MilliSeconds(23.12534573))			, TC("23125345ticks"));
	EXPECT_EQ(Format(TC("{}")	, ob::UUID())									, TC("00000000-0000-0000-0000-000000000000"));
	EXPECT_EQ(Format(TC("{}")	, Color::Cyan)									, TC("(0,1,1,1)"));
	EXPECT_EQ(Format(TC("{}")	, IntColor::Cyan)								, TC("(  0,255,255,255)"));
	EXPECT_EQ(Format(TC("{:#}")	, IntColor::Cyan)								, TC("FF00FFFF"));
	EXPECT_EQ(Format(TC("{}")	, HSV(Color::Cyan))								, TC("(180,1,1,1)"));
	EXPECT_EQ(Format(TC("{}")	, HSV::Cyan.toColor())							, TC("(0,1,1,1)"));
}
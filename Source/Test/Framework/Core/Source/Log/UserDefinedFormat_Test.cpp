///***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Misc/DateTime.h>
#include <Framework/Core/Misc/TimeSpan.h>

TEST(UserDefinedFormat, DateTime) {
	auto a = Format("{:.3}", Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f));
	EXPECT_EQ(Format("{}"	, Vec2(1, 2))									, "(1,2)");
	EXPECT_EQ(Format("{:.3}", Vec3(1.5235223f, 2.11f, -0.01f))				, "(1.52,2.11,-0.01)");
	EXPECT_EQ(Format("{:.3}", Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f))	, "(3.14,2.24e+06,-0.01,-0.00123)");
	EXPECT_EQ(Format("{}"	, IntVec2(4, 12))								, "(4,12)");
	EXPECT_EQ(Format("{}"	, IntVec3(474, 124, 12))						, "(474,124,12)");
	EXPECT_EQ(Format("{}"	, IntVec4(45, 1472, 313, 1444))					, "(45,1472,313,1444)");
	EXPECT_EQ(Format("{}"	, Quat::Identity)								, "(0,0,0,1)");
	EXPECT_EQ(Format("{}"	, Rot(180, 90, 45))								, "(180,90,45)");
	EXPECT_EQ(Format("{:.3}", Rot(180, 90, 45))								, "(180,90,45)");
	EXPECT_EQ(Format("{}"	, TimeSpan::Days(23.12534573))					, "23d03h00m");
	EXPECT_EQ(Format("{}"	, TimeSpan::Hours(23.12534573))					, "23h07m31s");
	EXPECT_EQ(Format("{}"	, TimeSpan::Minutes(23.12534573))				, "23m7.5207s");
	EXPECT_EQ(Format("{}"	, TimeSpan::Seconds(23.12534573))				, "23.125s");
	EXPECT_EQ(Format("{}"	, TimeSpan::MilliSeconds(23.12534573))			, "23125345ticks");
	EXPECT_EQ(Format("{}"	, ob::UUID())									, "00000000-0000-0000-0000-000000000000");
	EXPECT_EQ(Format("{}"	, Color::Cyan)									, "(0,1,1,1)");
	EXPECT_EQ(Format("{}"	, IntColor::Cyan)								, "(  0,255,255,255)");
	EXPECT_EQ(Format("{:#}"	, IntColor::Cyan)								, "FF00FFFF");
	EXPECT_EQ(Format("{}"	, HSV(Color::Cyan))								, "(180,1,1,1)");
	EXPECT_EQ(Format("{}"	, HSV::Cyan.toColor())							, "(0,1,1,1)");
}
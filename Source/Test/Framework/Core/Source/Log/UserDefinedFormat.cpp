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
	LOG_INFO("Vec2     :{}", Vec2(1, 2));
	LOG_INFO("Vec3     :{}", Vec3(1.5235223f, 2.11f, -0.01f));
	LOG_INFO("Vec4     :{}", Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f));
	LOG_INFO("IntVec2  :{}", IntVec2(4, 12));
	LOG_INFO("IntVec3  :{}", IntVec3(474, 124, 12));
	LOG_INFO("IntVec4  :{}", IntVec4(45, 1472, 313, 1444));
	LOG_INFO("Quat     :{}", Quat::Identity);
	LOG_INFO("Rot(Deg) :{}", Rot(180, 90, 45));
	LOG_INFO("Rot(Rad) :{:.3}", Rot(180, 90, 45));
	LOG_INFO("DateTime :{}", DateTime::Now());
	LOG_INFO("TimeSpan :{}", TimeSpan::Days(23.12534573));
	LOG_INFO("TimeSpan :{}", TimeSpan::Hours(23.12534573));
	LOG_INFO("TimeSpan :{}", TimeSpan::Minutes(23.12534573));
	LOG_INFO("TimeSpan :{}", TimeSpan::Seconds(23.12534573));
	LOG_INFO("TimeSpan :{}", TimeSpan::MilliSeconds(23.12534573));
	LOG_INFO("UUID     :{}", ob::UUID::Generate());
	LOG_INFO("Color    :{}", Color::Cyan);
	LOG_INFO("IntColor :{}", IntColor::Cyan);
	LOG_INFO("IntColor :{:#}", IntColor::Cyan);
	LOG_INFO("HSV	   :{}", HSV(Color::Cyan));
	LOG_INFO("HSV	   :{}", HSV::Cyan.toColor());
}
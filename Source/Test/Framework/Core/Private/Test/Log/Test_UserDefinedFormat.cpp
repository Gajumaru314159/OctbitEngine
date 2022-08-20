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
#include <Framework/Core/Misc/Duration.h>

TEST_GROUP(UserDefinedFormat) {
};

TEST(UserDefinedFormat, DataTime) {
	Vec2 vec2(1, 2);
	Vec3 vec3(1.5235223f, 2.11f, -0.01f);
	Vec4 vec4(3.1415f,2.235e6f,-0.01f,-123.5e-5f);
	IntVec2 ivec2(4, 12);
	IntVec3 ivec3(474, 124,12);
	IntVec4 ivec4(45, 1472,313,1444);

	Quat quat=Quat::Identity;
	Rot rot(Math::PI, Math::EPSILON,Math::HALF_PI);
	
	DateTime dt = DateTime::Now();
	Duration ts1 = Duration::Days(23.12534573);
	Duration ts2 = Duration::Hours(23.12534573);
	Duration ts3 = Duration::Minutes(23.12534573);
	Duration ts4 = Duration::Seconds(23.12534573);
	Duration ts5 = Duration::MilliSeconds(23.12534573);
	
	auto s = Format(TC("{}"), dt);

}
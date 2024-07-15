//***********************************************************
//! @file
//! @brief		Vector テスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Range.h>

using namespace ob;

TEST(Range, Constructor) {
	// デフォルトコンストラクタ
	{
		Range range;
		EXPECT_EQ(range.min, std::numeric_limits<f32>::lowest());
		EXPECT_EQ(range.max, std::numeric_limits<f32>::max());
	}
	// 範囲指定コンストラクタ
	{
		Range range(1.0f, 2.0f);
		EXPECT_EQ(range.min, 1.0f);
		EXPECT_EQ(range.max, 2.0f);
	}
}

TEST(Range, Operator) {
	// ==
	{
		EXPECT_TRUE(Range(1.0f, 2.0f) == Range(1.0f, 2.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f) == Range(1.0f, 3.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f) == Range(0.0f, 2.0f));
	}
	// !=
	{
		EXPECT_FALSE(Range(1.0f, 2.0f) != Range(1.0f, 2.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f) != Range(1.0f, 3.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f) != Range(0.0f, 2.0f));
	}
	// &
	{
		EXPECT_EQ(Range(1.0f, 2.0f) & Range(1.0f, 2.0f), Range(1.0f, 2.0f));
		EXPECT_EQ(Range(1.0f, 2.0f) & Range(1.0f, 3.0f), Range(1.0f, 2.0f));
		EXPECT_EQ(Range(1.0f, 2.0f) & Range(0.0f, 2.0f), Range(1.0f, 2.0f));
	}
	// &=
	{
		Range range(1.0f, 2.0f);
		range &= Range(1.0f, 2.0f);
		EXPECT_EQ(range, Range(1.0f, 2.0f));
		range = Range(1.0f, 2.0f);
		range &= Range(1.0f, 3.0f);
		EXPECT_EQ(range, Range(1.0f, 2.0f));
		range = Range(1.0f, 2.0f);
		range &= Range(0.0f, 2.0f);
		EXPECT_EQ(range, Range(1.0f, 2.0f));
	}
}

TEST(Range, Contains) {
	// contains
	{
		EXPECT_TRUE(Range(1.0f, 2.0f).contains(1.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f).contains(1.5f));
		EXPECT_TRUE(Range(1.0f, 2.0f).contains(2.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).contains(0.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).contains(2.1f));
	}
	// containsCC
	{
		EXPECT_TRUE(Range(1.0f, 2.0f).containsCC(1.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f).containsCC(1.5f));
		EXPECT_TRUE(Range(1.0f, 2.0f).containsCC(2.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsCC(0.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsCC(2.1f));
	}
	// containsOO
	{
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOO(1.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f).containsOO(1.5f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOO(2.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOO(0.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOO(2.1f));
	}
	// containsCO
	{
		EXPECT_TRUE(Range(1.0f, 2.0f).containsCO(1.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f).containsCO(1.5f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsCO(2.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsCO(0.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsCO(2.1f));
	}
	// containsOC
	{
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOC(1.0f));
		EXPECT_TRUE(Range(1.0f, 2.0f).containsOC(1.5f));
		EXPECT_TRUE(Range(1.0f, 2.0f).containsOC(2.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOC(0.0f));
		EXPECT_FALSE(Range(1.0f, 2.0f).containsOC(2.1f));
	}
}
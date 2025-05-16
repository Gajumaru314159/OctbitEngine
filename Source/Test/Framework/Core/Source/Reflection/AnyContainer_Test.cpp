//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/AnyContainer.h>

using namespace ob;

TEST(AnyContainer, Get) {

	struct Hoge {
		int a;
		float b;
	};


	struct Hoge2 {
		int& a;
		float& b;
	};

	AnyContainer container;

	container.get<int>() = 10;
	container.get<float>() = 20.0f;
	container.get<Hoge>() = Hoge{ 1, 2.0f };

	EXPECT_EQ(container.get<int>(), 10);
	EXPECT_EQ(container.get<float>(), 20.0f);
	EXPECT_EQ(container.get<Hoge>().a, 1);
	EXPECT_EQ(container.get<Hoge>().b, 2.0f);

	container.get<int>() = 11;
	container.get<float>() = 22.0f;
	container.get<Hoge>().a = 11;
	container.get<Hoge>().b = 22.0f;
	
	EXPECT_EQ(container.get<int>(), 11);
	EXPECT_EQ(container.get<float>(), 22.0f);
	EXPECT_EQ(container.get<Hoge>().a, 11);
	EXPECT_EQ(container.get<Hoge>().b, 22.0f);
	
	// デフォルトコンストラクタのないクラスは追加できません
	// container.get<Hoge2>();

}

TEST(AnyContainer, Contains) {
	struct Hoge {
		int a;
		float b;
	};
	AnyContainer container;
	EXPECT_FALSE(container.contains<int>());
	EXPECT_FALSE(container.contains<float>());
	EXPECT_FALSE(container.contains<Hoge>());
	container.get<int>() = 10;
	container.get<float>() = 20.0f;
	container.get<Hoge>() = Hoge{ 1, 2.0f };
	EXPECT_TRUE(container.contains<int>());
	EXPECT_TRUE(container.contains<float>());
	EXPECT_TRUE(container.contains<Hoge>());
	container.get<int>() = 11;
	container.get<float>() = 22.0f;
	container.get<Hoge>().a = 11;
	container.get<Hoge>().b = 22.0f;
	EXPECT_TRUE(container.contains<int>());
	EXPECT_TRUE(container.contains<float>());
	EXPECT_TRUE(container.contains<Hoge>());
}
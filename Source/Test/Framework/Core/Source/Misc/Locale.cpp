//***********************************************************
//! @file
//! @brief		Locale のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Locale.h>

using namespace ob;

TEST(Locale, Constructor)
{
	{
		Locale locale("");
		EXPECT_EQ(locale.size(), 0);
		EXPECT_EQ(locale.at(0), "");
	}
	{
		Locale locale("en");
		EXPECT_EQ(locale.size(), 1);
		EXPECT_EQ(locale.at(0), "en");
		EXPECT_EQ(locale.at(1), "");
	}
	{
		Locale locale("ja-JP");
		EXPECT_EQ(locale.size(), 2);
		EXPECT_EQ(locale.at(0), "ja");
		EXPECT_EQ(locale.at(1), "JP");
		EXPECT_EQ(locale.at(2), "");
	}
	{
		Locale locale("language-script-region-variant-extension-privateuse");
		EXPECT_EQ(locale.tag(), "language-script-region-variant-extension-privateuse");
		EXPECT_EQ(locale.size(), 6);
		EXPECT_EQ(locale.language(), "language");
		EXPECT_EQ(locale.script(), "script");
		EXPECT_EQ(locale.region(), "region");
		EXPECT_EQ(locale.variant(), "variant");
		EXPECT_EQ(locale.extension(), "extension");
		EXPECT_EQ(locale.privateuse(), "privateuse");
	}
}

TEST(Locale, Contains)
{
	EXPECT_TRUE(Locale("en").contains(Locale("en-US")));
	EXPECT_FALSE(Locale("en-US").contains(Locale("en")));
	EXPECT_TRUE(Locale("en-US").contains(Locale("en-US")));
}

TEST(Locale, System)
{
	EXPECT_FALSE(Locale::System().empty());
}
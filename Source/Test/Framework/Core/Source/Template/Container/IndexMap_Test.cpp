//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <string>
#include <Framework/Core/Template/Container/IndexMap.h>

TEST(IndexMap, InsertAndIterationOrder) {
	core::IndexMap<int, const char*> map;
	auto [it1, inserted1] = map.insert({ 3, "three" });
	auto [it2, inserted2] = map.insert({ 1, "one" });
	auto [it3, inserted3] = map.insert({ 2, "two" });
	auto [it4, inserted4] = map.insert({ 3, "THREE" });

	EXPECT_TRUE(inserted1);
	EXPECT_TRUE(inserted2);
	EXPECT_TRUE(inserted3);
	EXPECT_FALSE(inserted4);
	EXPECT_STREQ(it4->second, "three");

	int expectedKeys[] = { 3, 1, 2 };
	const char* expectedValues[] = { "three", "one", "two" };

	int index = 0;
	for (const auto& entry : map) {
		EXPECT_EQ(expectedKeys[index], entry.first);
		EXPECT_STREQ(expectedValues[index], entry.second);
		index++;
	}
	EXPECT_EQ(map.size(), 3u);
}

TEST(IndexMap, IndexAccess) {
	core::IndexMap<std::string, int> map;
	map.insert({ "apple", 1 });
	map.try_emplace("banana", 2);
	map["cherry"] = 3;

	EXPECT_EQ(map.index_of("apple"), 0u);
	EXPECT_EQ(map.index_of("banana"), 1u);
	EXPECT_EQ(map.index_of("cherry"), 2u);
	EXPECT_EQ(map.index_of("orange"), decltype(map)::npos);

	auto first = map.at_index(0);
	EXPECT_EQ(first.first, "apple");
	EXPECT_EQ(first.second, 1);

	map.at("banana") = 20;
	EXPECT_EQ(map.at("banana"), 20);
	EXPECT_EQ(map.at_index(1).second, 20);
}

TEST(IndexMap, EraseMaintainsCompactIndices) {
	core::IndexMap<int, std::string> map;
	map.insert({ 10, "ten" });
	map.insert({ 20, "twenty" });
	map.insert({ 30, "thirty" });
	map.insert({ 40, "forty" });

	EXPECT_TRUE(map.erase(20));
	EXPECT_FALSE(map.contains(20));
	EXPECT_EQ(map.size(), 3u);
	EXPECT_EQ(map.index_of(30), 1u);
	EXPECT_EQ(map.index_of(40), 2u);

	auto it = map.find(30);
	it = map.erase(it);
	EXPECT_EQ(it->first, 40);
	EXPECT_EQ(map.index_of(40), 1u);

	map.erase(map.begin(), map.end());
	EXPECT_TRUE(map.empty());
}

TEST(IndexMap, InsertOrAssign) {
	core::IndexMap<int, std::string> map;

	auto [it1, inserted1] = map.insert_or_assign(1, "one");
	auto [it2, inserted2] = map.insert_or_assign(2, "two");
	auto [it3, inserted3] = map.insert_or_assign(1, "ONE");

	EXPECT_TRUE(inserted1);
	EXPECT_TRUE(inserted2);
	EXPECT_FALSE(inserted3);
	EXPECT_EQ(map.size(), 2u);
	EXPECT_EQ(map.index_of(1), 0u);
	EXPECT_EQ(map.at(1), "ONE");
	EXPECT_EQ(map.at_index(0).first, 1);
	EXPECT_EQ(map.at_index(0).second, "ONE");
}

TEST(IndexMap, TryGetAndTryAtIndex) {
	core::IndexMap<int, std::string> map;
	map.insert({ 4, "four" });
	map.insert({ 5, "five" });

	auto* val4 = map.try_get(4);
	ASSERT_NE(val4, nullptr);
	EXPECT_EQ(*val4, "four");

	EXPECT_EQ(map.try_get(99), nullptr);

	auto* at0 = map.try_at_index(0);
	ASSERT_NE(at0, nullptr);
	EXPECT_EQ(*at0, "four");
	EXPECT_EQ(map.at_index(0).first, 4);

	EXPECT_EQ(map.try_at_index(20), nullptr);
}

TEST(IndexMap, DataProvidesContiguousValues) {
	core::IndexMap<int, int> map;
	for (int i = 0; i < 5; ++i) {
		map.insert({ i, i * 2 });
	}

	int* values = map.data();
	ASSERT_NE(values, nullptr);
	for (int i = 0; i < 5; ++i) {
		EXPECT_EQ(values[i], map.at(i));
	}

	values[2] = 99;
	EXPECT_EQ(map.at(2), 99);
}

TEST(IndexMap, SwapEraseVariants) {
	core::IndexMap<int, std::string> map;
	map.insert({ 1, "one" });
	map.insert({ 2, "two" });
	map.insert({ 3, "three" });

	auto removed = map.swap_erase_index(0);
	ASSERT_TRUE(removed.has_value());
	EXPECT_EQ(removed->first, 1);
	EXPECT_FALSE(map.contains(1));
	EXPECT_EQ(map.size(), 2u);

	removed = map.swap_erase(2);
	ASSERT_TRUE(removed.has_value());
	EXPECT_EQ(removed->first, 2);
	EXPECT_FALSE(map.contains(2));
	EXPECT_EQ(map.size(), 1u);

	EXPECT_FALSE(map.swap_erase(100).has_value());
}

TEST(IndexMap, EraseEntryReturnsValue) {
	core::IndexMap<int, std::string> map;
	map.insert({ 10, "ten" });
	map.insert({ 11, "eleven" });

	auto removed = map.erase_entry(10);
	ASSERT_TRUE(removed.has_value());
	EXPECT_EQ(removed->first, 10);
	EXPECT_EQ(removed->second, "ten");
	EXPECT_FALSE(map.contains(10));
	EXPECT_EQ(map.size(), 1u);

	EXPECT_FALSE(map.erase_entry(42).has_value());
}

TEST(IndexMap, PopBackAndShrinkToFit) {
	core::IndexMap<int, int> map;
	for (int i = 0; i < 6; ++i) {
		map.insert({ i, i * 10 });
	}

	auto popped = map.pop_back();
	ASSERT_TRUE(popped.has_value());
	EXPECT_EQ(popped->first, 5);
	EXPECT_EQ(popped->second, 50);
	EXPECT_EQ(map.size(), 5u);
	EXPECT_FALSE(map.contains(5));

	auto previousCapacity = map.capacity();
	map.shrink_to_fit();
	EXPECT_LE(map.capacity(), previousCapacity);
	for (int i = 0; i < 5; ++i) {
		EXPECT_TRUE(map.contains(i));
		EXPECT_EQ(map.at(i), i * 10);
	}
}

//***********************************************************
//! @file
//! @brief		BitFlags テスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Template/Container/FixedArray.h>

TEST(FixedArray, Constructors) {
	FixedArray<int, 100> fakeEmpty, fake(5, 10), fakeCopy(fake), fakeArray{ 2, 3, 4, 5, 7 };
	FixedArray<int, 100> realEmpty, real(5, 10), realCopy(real), realArray{ 2, 3, 4, 5, 7 };

	ob::core::is_iterator<decltype(fake.begin())>::value;
	EXPECT_TRUE(fakeEmpty.size() == realEmpty.size());
	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fakeCopy.size() == realCopy.size());
	EXPECT_TRUE(fakeArray.size() == realArray.size());
	for (int i = 0; i < fake.size(); i++) {
		EXPECT_TRUE(fake[i] == real[i]);
		EXPECT_TRUE(fakeCopy[i] == realCopy[i]);
		EXPECT_TRUE(fakeArray[i] == realArray[i]);
	}
}

TEST(FixedArray, PushBack) {
	FixedArray<int, 100> fake(3, 10);
	FixedArray<int, 100> real(3, 10);

	fake.push_back(4);
	real.push_back(4);

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	EXPECT_TRUE(fake.back() == fake.back());
}

TEST(FixedArray, Assign) {
	FixedArray<int, 100> fake(10, 55);
	FixedArray<int, 100> real(10, 55);

	fake.assign(5, 3);
	real.assign(5, 3);

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);

	fake.assign(20, 3);
	real.assign(20, 3);

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);

}

TEST(FixedArray, ElementAccess) {
	FixedArray<int, 100> fake(10);
	FixedArray<int, 100> real(10);

	for (int i = 0; i < 10; i++) {
		fake[i] = i;
		real[i] = i;
	}

	EXPECT_TRUE(fake.at(5) == real.at(5));
	EXPECT_TRUE(fake.front() == real.front());
	EXPECT_TRUE(fake.back() == real.back());
}

TEST(FixedArray, Clear) {
	FixedArray<int, 100> fake(5, 55);
	FixedArray<int, 100> real(5, 55);

	fake.clear();
	real.clear();

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
}

//TEST(FixedArray, Insert) {
//	FixedArray<int, 100> fake(10, 5);
//	FixedArray<int, 100> real(10, 5);
//
//	auto itfake = fake.begin() + 2;
//	auto itreal = real.begin() + 2;
//
//
//	itfake = fake.insert(itfake, 10);
//	itreal = real.insert(itreal, 10);
//
//	EXPECT_TRUE(fake.size() == real.size());
//	for (int i = 0; i < fake.size(); i++)
//		EXPECT_TRUE(fake[i] == real[i]);
//
//	int ten = 10;
//	itfake = fake.insert(itfake, 3, ten);
//	itreal = real.insert(itreal, 3, ten);
//
//	EXPECT_TRUE(fake.size() == real.size());
//	for (int i = 0; i < fake.size(); i++)
//		EXPECT_TRUE(fake[i] == real[i]);
//
//}

TEST(FixedArray, Erase) {
	FixedArray<int, 100> fake(10);
	FixedArray<int, 100> real(10);


	for (int i = 0; i < 10; i++) {
		fake[i] = i + 1;
		real[i] = i + 1;
	}

	auto itfake = fake.begin() + 2;
	itfake = fake.erase(itfake);
	auto itreal = real.begin() + 2;
	itreal = real.erase(itreal);

	EXPECT_TRUE(fake[2] == real[2]);
	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());

	auto itfake1 = fake.begin() + 2;
	auto itfake2 = fake.begin() + 5;
	itfake2 = fake.erase(itfake1, itfake2);
	auto itreal1 = real.begin() + 2;
	auto itreal2 = real.begin() + 5;
	itreal2 = real.erase(itreal1, itreal2);

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);

}

TEST(FixedArray, PopBack) {
	FixedArray<int, 100> fake(10);
	FixedArray<int, 100> real(10);

	for (int i = 0; i < 10; i++)
		fake[i] = real[i] = i + 1;

	fake.pop_back();
	real.pop_back();

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);
}

TEST(FixedArray, Resize) {
	FixedArray<int, 100> fake(10, 10);
	FixedArray<int, 100> real(10, 10);

	fake.resize(5);
	real.resize(5);

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);

	fake.resize(5, 12);
	real.resize(5, 12);

	EXPECT_TRUE(fake.size() == real.size());
	EXPECT_TRUE(fake.capacity() == real.capacity());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);

	fake.resize(15, 12);
	real.resize(15, 12);

	EXPECT_TRUE(fake.size() == real.size());
	for (int i = 0; i < fake.size(); i++)
		EXPECT_TRUE(fake[i] == real[i]);
}

TEST(FixedArray, Swap) {
	FixedArray<int, 100> fake1(10, 10), fake2(5, 5);
	FixedArray<int, 100> real1(10, 10), real2(5, 5);

	fake1.swap(fake2);
	real1.swap(real2);

	EXPECT_TRUE(fake1.size() == real1.size());
	for (int i = 0; i < fake1.size(); i++)
		EXPECT_TRUE(fake1[i] == real1[i]);

	fake2.swap(fake1);
	real2.swap(real1);

	EXPECT_TRUE(fake2.size() == real2.size());
	for (int i = 0; i < fake2.size(); i++)
		EXPECT_TRUE(fake2[i] == real2[i]);
}

TEST(FixedArray, Operators) {
	FixedArray<int, 100> fake1(10, 5), fake2(10, 5), fake3(10, 3), fake4(4, 2);

	EXPECT_TRUE((fake1 == fake2) == true);
	EXPECT_TRUE((fake1 == fake3) == false);

	EXPECT_TRUE((fake1 != fake2) == false);
	EXPECT_TRUE((fake1 != fake3) == true);

	EXPECT_TRUE((fake1 < fake2) == false);
	EXPECT_TRUE((fake4 < fake2) == true);
	EXPECT_TRUE((fake1 < fake3) == false);

	EXPECT_TRUE((fake1 > fake2) == false);
	EXPECT_TRUE((fake4 > fake2) == false);
	EXPECT_TRUE((fake1 > fake3) == true);

	EXPECT_TRUE((fake1 <= fake2) == true);
	EXPECT_TRUE((fake4 <= fake2) == true);
	EXPECT_TRUE((fake1 <= fake3) == false);

	EXPECT_TRUE((fake1 >= fake2) == true);
	EXPECT_TRUE((fake4 >= fake2) == false);
	EXPECT_TRUE((fake1 >= fake3) == true);

}
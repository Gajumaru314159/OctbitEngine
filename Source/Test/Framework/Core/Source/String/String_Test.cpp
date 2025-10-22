//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/String/String.h>
#include <utf8/checked.h>

using namespace ob;

TEST(String, MemberTypes) {
	// value_typeのテスト
	{
		String::value_type value = 't';
		EXPECT_EQ(value, 't');
	}
	// size_typeのテスト
	{
		String::size_type value = 4;
		EXPECT_EQ(value, 4);
	}
	// difference_typeのテスト
	{
		String::difference_type value = 4;
		EXPECT_EQ(value, 4);
	}
	// referenceのテスト
	{
		String str("test");
		String::reference value = str[0];
		EXPECT_EQ(value, 't');
	}
	// const_referenceのテスト
	{
		const String str("test");
		String::const_reference value = str[0];
		EXPECT_EQ(value, 't');
	}
	// pointerのテスト
	{
		String str("test");
		String::pointer value = &str[0];
		EXPECT_EQ(*value, 't');
	}
	// const_pointerのテスト
	{
		const String str("test");
		String::const_pointer value = &str[0];
		EXPECT_EQ(*value, 't');
	}
	// iteratorのテスト
	{
		String str("test");
		String::iterator value = str.begin();
		EXPECT_EQ(*value, 't');
	}
	// const_iteratorのテスト
	{
		const String str("test");
		String::const_iterator value = str.begin();
		EXPECT_EQ(*value, 't');
	}
	// reverse_iteratorのテスト
	{
		String str("test");
		String::reverse_iterator value = str.rbegin();
		EXPECT_EQ(*value, 't');
	}
	// const_reverse_iteratorのテスト
	{
		const String str("test");
		String::const_reverse_iterator value = str.rbegin();
		EXPECT_EQ(*value, 't');
	}
}

// Stringのコンストラクタのテスト
TEST(String, Constructor) {
	// デフォルトコンストラクタ
	{
		String str;
		EXPECT_EQ(str.empty(), true);
	}
	// コピーコンストラクタ
	{
		String str1("test");
		String str2(str1);
		EXPECT_EQ(str2, "test");
	}
	// ムーブコンストラクタ
	{
		String str1("test");
		String str2(std::move(str1));
		EXPECT_EQ(str2, "test");
		EXPECT_EQ(str1.empty(), true);
	}
	// std::stringのムーブコンストラクタ
	{
		std::string str1("test");
		String str2(std::move(str1));
		EXPECT_EQ(str2, "test");
		EXPECT_EQ(str1.empty(), true);
	}
	// リテラルコンストラクタ
	{
		String str("test");
		EXPECT_EQ(str, "test");
	}
	// std::stringによるコンストラクタ
	{
		std::string str("test");
		String str2(str);
		EXPECT_EQ(str2, "test");
	}
	// std::string_viewによるコンストラクタ
	{
		std::string_view str("test");
		String str2(str);
		EXPECT_EQ(str2, "test");
	}
	// char*によるコンストラクタ
	{
		const char* str = "test";
		String str2(str);
		EXPECT_EQ(str2, "test");
	}
	// charによるコンストラクタ
	{
		char str[] = "test";
		String str2(str);
		EXPECT_EQ(str2, "test");
	}
	// Stringとposによるコンストラクタ
	{
		String str("test");
		String str2(str, 2);
		EXPECT_EQ(str2, "st");
	}
	// std::stringとposによるコンストラクタ
	{
		std::string str("test");
		String str2(str, 2);
		EXPECT_EQ(str2, "st");
	}
	// Stringとposとlenによるコンストラクタ
	{
		String str("test");
		String str2(str, 1, 2);
		EXPECT_EQ(str2, "es");
	}
	// std::stringとposとlenによるコンストラクタ
	{
		std::string str("test");
		String str2(str, 1, 2);
		EXPECT_EQ(str2, "es");
	}
	// const char*とlenによるコンストラクタ
	{
		const char* str = "test";
		String str2(str, 2);
		EXPECT_EQ(str2, "te");
	}
	// initializer_listによるコンストラクタ
	{
		String str({ 't', 'e', 's', 't' });
		EXPECT_EQ(str, "test");
	}
	// sizeとcによるコンストラクタ
	{
		String str(4, 't');
		EXPECT_EQ(str, "tttt");
	}
}

TEST(String, OperatorAssign) {
	// Stringの代入演算子のテスト
	{
		String str;
		str = "test";
		EXPECT_EQ(str, "test");
	}
	// std::stringの代入演算子
	{
		String str;
		std::string str2 = "test";
		str = str2;
		EXPECT_EQ(str, "test");
	}
	// Stringのムーブ代入演算子
	{
		String str;
		String str2("test");
		str = std::move(str2);
		EXPECT_EQ(str, "test");
	}
	// std::stringのムーブ代入演算子
	{
		String str;
		std::string str2("test");
		str = std::move(str2);
		EXPECT_EQ(str, "test");
	}
	// char*の代入演算子
	{
		String str;
		const char* str2 = "test";
		str = str2;
		EXPECT_EQ(str, "test");
	}
	// char[]の代入演算子
	{
		String str;
		char str2[] = "test";
		str = str2;
		EXPECT_EQ(str, "test");
	}
	// initializer_listの代入演算子
	{
		String str;
		str = { 't', 'e', 's', 't' };
		EXPECT_EQ(str, "test");
	}
	// sizeとcの代入演算子
	{
		String str;
		str = String(4, 't');
		EXPECT_EQ(str, "tttt");
	}
	// std::string_viewの代入演算子
	{
		String str;
		std::string_view str2 = "test";
		str = str2;
		EXPECT_EQ(str, "test");
	}
}

TEST(String, Convert) {
	// StringViewへの変換
	{
		String str("test");
		StringView str2 = str;
		EXPECT_EQ(str2, "test");
	}
	// std::stringへの変換
	{
		String str("test");
		std::string str2 = str;
		EXPECT_EQ(str2, "test");
	}
	// std::string_viewへの変換
	{
		String str("test");
		std::string_view str2 = str;
		EXPECT_EQ(str2, "test");
	}
}

TEST(String, Iterator) {
	// beginとendのテスト
	{
		String str("test");
		EXPECT_EQ(*str.begin(), 't');
		EXPECT_NE(str.begin(),str.end());
	}
	// cbeginとcendのテスト
	{
		String str("test");
		EXPECT_EQ(*str.cbegin(), 't');
		EXPECT_NE(str.cbegin(), str.cend());
	}
	// rbeginとrendのテスト
	{
		String str("test");
		EXPECT_EQ(*str.rbegin(), 't');
		EXPECT_NE(str.rbegin(), str.rend());
	}
	// crbeginとcrendのテスト
	{
		String str("test");
		EXPECT_EQ(*str.crbegin(), 't');
		EXPECT_NE(str.crbegin(), str.crend());
	}
	// 拡張for文のテスト
	{
		String str("test");
		s32 index = 0;
		for (auto c : str) {
			if (index==0) EXPECT_EQ(c, 't');
			if (index==1) EXPECT_EQ(c, 'e');
			if (index==2) EXPECT_EQ(c, 's');
			if (index==3) EXPECT_EQ(c, 't');
		}
		EXPECT_EQ(index, 4);
	}
	// 拡張for分のテスト(const版)
	{
		const String str("test");
		s32 index = 0;
		for (auto c : str) {
			if (index==0) EXPECT_EQ(c, 't');
			if (index==1) EXPECT_EQ(c, 'e');
			if (index==2) EXPECT_EQ(c, 's');
			if (index==3) EXPECT_EQ(c, 't');
		}
		EXPECT_EQ(index, 4);
	}
}

TEST(String, Capacity) {
	// sizeのテスト
	{
		String str("test");
		EXPECT_EQ(str.size(), 4);
	}
	// lengthのテスト
	{
		String str("test");
		EXPECT_EQ(str.length(), 4);
	}
	// max_sizeのテスト
	{
		String str("test");
		EXPECT_TRUE(0 <= str.max_size());
	}
	// resizeのテスト
	{
		String str("test");
		str.resize(2);
		EXPECT_EQ(str, "te");
	}
	// resizeのテスト(n,c)
	{
		String str("test");
		str.resize(6, 't');
		EXPECT_EQ(str, "testtt");
	}
	// capacityのテスト
	{
		String str("test");
		EXPECT_TRUE(str.size() <= str.capacity());
	}
	// reserveのテスト
	{
		String str("test");
		str.reserve(100);
		EXPECT_TRUE(100 <= str.capacity());
	}
	// shrink_to_fitのテスト
	{
		String str("test");
		str.reserve(100);
		str.shrink_to_fit();
		// 縮小は強制されていないため呼び出しだけテスト
	}
	// clearのテスト
	{
		String str("test");
		str.clear();
		EXPECT_EQ(str.empty(), true);
	}
	// emptyのテスト
	{
		String str;
		EXPECT_EQ(str.empty(), true);
	}
	// size_bytesのテスト
	{
		String str("test");
		EXPECT_EQ(str.size_bytes(), 5);
	}
}

TEST(String, Access) {
	// atのテスト
	{
		String str("test");
		EXPECT_EQ(str.at(0), 't');
	}
	// atのテスト(const)
	{
		const String str("test");
		EXPECT_EQ(str.at(0), 't');
	}
	// atの例外テスト
	{
		String str("test");
		//EXPECT_THROW(str.at(4), std::out_of_range);
	}
	// operator[]のテスト
	{
		String str("test");
		EXPECT_EQ(str[0], 't');
	}
	// operator[]のテスト(const)
	{
		const String str("test");
		EXPECT_EQ(str[0], 't');
	}
	// frontのテスト
	{
		String str("test");
		EXPECT_EQ(str.front(), 't');
	}
	// frontのテスト(const)
	{
		const String str("test");
		EXPECT_EQ(str.front(), 't');
	}
	// backのテスト
	{
		String str("test");
		EXPECT_EQ(str.back(), 't');
	}
	// backのテスト(const)
	{
		const String str("test");
		EXPECT_EQ(str.back(), 't');
	}
}

TEST(String, Append) {
	// appendのテスト(String)
	{
		String str("test");
		String str2("test");
		str.append(str2);
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(std::string)
	{
		String str("test");
		std::string str2("test");
		str.append(str2);
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(char*)
	{
		String str("test");
		const char* str2 = "test";
		str.append(str2);
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(リテラル)
	{
		String str("test");
		str.append("test");
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(char[])
	{
		String str("test");
		char str2[] = "test";
		str.append(str2);
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(char*,size)
	{
		String str("test");
		const char* str2 = "test";
		str.append(str2, 2);
		EXPECT_EQ(str, "testte");
	}
	// appendのテスト(char[],size)
	{
		String str("test");
		char str2[] = "test";
		str.append(str2, 2);
		EXPECT_EQ(str, "testte");
	}
	// appendのテスト(size,c)
	{
		String str("test");
		str.append(2, 't');
		EXPECT_EQ(str, "testtt");
	}
	// appendのテスト(std::initializer_list)
	{
		String str("test");
		str.append({ 't', 'e' });
		EXPECT_EQ(str, "testte");
	}
	// appendのテスト(StringView)
	{
		String str("test");
		StringView str2("test");
		str.append(str2);
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(std::string_view)
	{
		String str("test");
		std::string_view str2("test");
		str.append(str2);
		EXPECT_EQ(str, "testtest");
	}
	// appendのテスト(イテレータ)
	{
		String str("test");
		String str2("test");
		str.append(str2.begin(), str2.end());
		EXPECT_EQ(str, "testtest");
	}
	// push_frontのテスト
	{
		String str("test");
		str.push_front('t');
		EXPECT_EQ(str, "ttest");
	}
	// push_backのテスト
	{
		String str("test");
		str.push_back('t');
		EXPECT_EQ(str, "testt");
	}
}

TEST(String, Assign) {
	// Stringのassignのテスト
	{
		String str;
		str.assign("test");
		EXPECT_EQ(str, "test");
	}
	// std::stringのassign
	{
		String str;
		std::string str2 = "test";
		str.assign(str2);
		EXPECT_EQ(str, "test");
	}
	// Stringのムーブassign
	{
		String str;
		String str2("test");
		str.assign(std::move(str2));
		EXPECT_EQ(str, "test");
	}
	// std::stringのムーブassign
	{
		String str;
		std::string str2("test");
		str.assign(std::move(str2));
		EXPECT_EQ(str, "test");
	}
	// char*のassign
	{
		String str;
		const char* str2 = "test";
		str.assign(str2);
		EXPECT_EQ(str, "test");
	}
	// char[]のassign
	{
		String str;
		char str2[] = "test";
		str.assign(str2);
		EXPECT_EQ(str, "test");
	}
	// initializer_listのassign
	{
		String str;
		str.assign({ 't', 'e', 's', 't' });
		EXPECT_EQ(str, "test");
	}
	// sizeとcのassign
	{
		String str;
		str.assign(4, 't');
		EXPECT_EQ(str, "tttt");
	}
	// std::string_viewのassign
	{
		String str;
		std::string_view str2 = "test";
		str.assign(str2);
		EXPECT_EQ(str, "test");
	}
}

TEST(String, Insert) {
	// insertのテスト(pos,String)
	{
		String str("test");
		String str2("test");
		str.insert(2, str2);
		EXPECT_EQ(str, "tetestst");
	}
	// insertのテスト(pos,std::string)
	{
		String str("test");
		std::string str2("test");
		str.insert(2, str2);
		EXPECT_EQ(str, "tetestst");
	}
	// insertのテスト(pos,char*)
	{
		String str("test");
		const char* str2 = "test";
		str.insert(2, str2);
		EXPECT_EQ(str, "tetestst");
	}
	// insertのテスト(pos,リテラル)
	{
		String str("test");
		str.insert(2, "test");
		EXPECT_EQ(str, "tetestst");
	}
	// insertのテスト(pos,char[])
	{
		String str("test");
		char str2[] = "test";
		str.insert(2, str2);
		EXPECT_EQ(str, "tetestst");
	}
	// // insertのテスト(pos,char*,size)
	// {
	// 	String str("test");
	// 	const char* str2 = "test";
	// 	str.insert(2, str2, 2);
	// 	EXPECT_EQ(str, "tetestst");
	// }
	// // insertのテスト(pos,char[],size)
	// {
	// 	String str("test");
	// 	char str2[] = "test";
	// 	str.insert(2, str2, 2);
	// 	EXPECT_EQ(str, "tetestst");
	// }
	// insertのテスト(pos,size,c)
	{
		String str("test");
		str.insert(2, 2, 't');
		EXPECT_EQ(str, "tettst");
	}
	// insertのテスト(pos,std::initializer_list)
	{
		String str("test");
		str.insert(2, { 't', 'e' });
		EXPECT_EQ(str, "tetest");
	}
	// insertのテスト(pos,StringView)
	{
		String str("test");
		StringView str2("test");
		str.insert(2, str2);
		EXPECT_EQ(str, "tetestst");
	}
}

TEST(String, Erase) {
	// eraseのテスト(pos)
	{
		String str("test");
		str.erase(2);
		EXPECT_EQ(str, "te");
	}
	// eraseのテスト(pos,len)
	{
		String str("test");
		str.erase(1, 2);
		EXPECT_EQ(str, "tt");
	}
	// eraseのテスト(iterator)
	{
		String str("test");
		str.erase(str.begin() + 1);
		EXPECT_EQ(str, "tst");
	}
	// eraseのテスト(iterator,iterator)
	{
		String str("test");
		str.erase(str.begin() + 1, str.begin() + 3);
		EXPECT_EQ(str, "tt");
	}
	// pop_frontのテスト
	{
		String str("test");
		str.pop_front();
		EXPECT_EQ(str, "est");
	}
	// pop_frontのテスト(数指定)
	{
		String str("test");
		str.pop_front(2);
		EXPECT_EQ(str, "st");
	}
	// pop_backのテスト
	{
		String str("test");
		str.pop_back();
		EXPECT_EQ(str, "tes");
	}
	// pop_backのテスト(数指定)
	{
		String str("test");
		str.pop_back(2);
		EXPECT_EQ(str, "te");
	}
	// removeのテスト
	{
		String str("test");
		str.remove('t');
		EXPECT_EQ(str, "es");
	}
	// removeのテスト(文字列)
	{
		String str("test");
		str.remove("t");
		EXPECT_EQ(str, "es");
	}
	// trimのテスト
	{
		String str("  test  ");
		str.trim();
		EXPECT_EQ(str, "test");
	}
	// trimのテスト(\t \n \r \f \v)
	{
		String str("\t\ntest\r\f\v");
		str.trim();
		EXPECT_EQ(str, "test");
	}
}

TEST(String, Replase) {
	// 文字置換のテスト
	{
		String str("test");
		str.replace('t', 'a');
		EXPECT_EQ(str, "aesa");
	}
	// 文字列置換のテスト
	{
		String str("test");
		str.replace("t", "a");
		EXPECT_EQ(str, "aesa");
	}
	// String置換のテスト
	{
		String str("test");
		String str2("t");
		String str3("a");
		str.replace(str2, str3);
		EXPECT_EQ(str, "aesa");
	}
	// StringView置換のテスト
	{
		String str("test");
		StringView str2("t");
		StringView str3("a");
		str.replace(str2, str3);
		EXPECT_EQ(str, "aesa");
	}
	// ois,n,repのテスト
	{
		String str("test");
		str.replace(1, 2, "a");
		EXPECT_EQ(str, "tat");
	}
	// first,last,repのテスト
	{
		String str("test");
		str.replace(str.begin() + 1, str.begin() + 3, "a");
		EXPECT_EQ(str, "tat");
	}
	// first,last,first,lastのテスト
	{
		String str("test");
		String str2("apple");
		str.replace(str.begin() + 1, str.begin() + 3, str2.begin(), str2.begin() + 2);
		EXPECT_EQ(str, "tapt");
	}
}

TEST(String, Swap) {
	// swapのテスト
	{
		String str("test");
		String str2("tset");
		str.swap(str2);
		EXPECT_EQ(str, "tset");
		EXPECT_EQ(str2, "test");
	}
	// swapのテスト(std::string)
	{
		String str("test");
		std::string str2("tset");
		str.swap(str2);
		EXPECT_EQ(str, "tset");
		EXPECT_EQ(str2, "test");
	}
}

TEST(String, DataAccess) {
	// c_strのテスト
	{
		String str("test");
		EXPECT_EQ(strcmp(str.c_str(), "test"),0);
	}
	// dataのテスト
	{
		String str("test");
		EXPECT_EQ(strcmp(str.data(), "test"), 0);
	}
	// strのテスト
	{
		String str("test");
		EXPECT_EQ(str.str(), "test");
	}
	// copyのテスト
	{
		String str("test");
		char buf[5];
		str.copy(buf, 4);
		buf[4] = '\0';
		EXPECT_EQ(strcmp(buf, "test"), 0);
	}
	// copyのテスト(dst,n,pos)
	{
		String str("test");
		char buf[5];
		str.copy(buf, 2, 1);
		buf[2] = '\0';
		EXPECT_EQ(strcmp(buf, "es"), 0);
	}
}

TEST(String, SubStr) {
	// substrのテスト
	{
		String str("test");
		EXPECT_EQ(str.substr(1), "est");
	}
	// substrのテスト(pos,len)
	{
		String str("test");
		EXPECT_EQ(str.substr(1, 2), "es");
	}
	// rsubstrのテスト
	{
		String str("test");
		EXPECT_EQ(str.rsubstr(2), "te");
	}
	// rsubstrのテスト(pos,len)
	{
		String str("test");
		EXPECT_EQ(str.rsubstr(1, 2), "es");
	}
	// substr_rangeのテスト
	{
		String str("test");
		EXPECT_EQ(str.substr_range(1, 2), "es");
	}
}

TEST(String, Find) {
	// findのテスト
	{
		String str("test");
		EXPECT_EQ(str.find('t'), 0);
	}
	// findのテスト(pos)
	{
		String str("test");
		EXPECT_EQ(str.find('t', 1), 3);
	}
	// findのテスト(String)
	{
		String str("test");
		String str2("t");
		EXPECT_EQ(str.find(str2), 0);
	}
	// findのテスト(String,pos)
	{
		String str("test");
		String str2("t");
		EXPECT_EQ(str.find(str2, 1), 3);
	}
	// findのテスト(char*)
	{
		String str("test");
		const char* str2 = "t";
		EXPECT_EQ(str.find(str2), 0);
	}
	// findのテスト(char*,pos)
	{
		String str("test");
		const char* str2 = "t";
		EXPECT_EQ(str.find(str2, 1), 3);
	}
	// findのテスト(char*,pos,len)
	{
		String str("test");
		const char* str2 = "t";
		EXPECT_EQ(str.find(str2, 1, 1), 3);
	}
	// findのテスト(リテラル)
	{
		String str("test");
		EXPECT_EQ(str.find("t"), 0);
	}
	// findのテスト(リテラル,pos)
	{
		String str("test");
		EXPECT_EQ(str.find("t", 1), 3);
	}
	// findのテスト(リテラル,pos,len)
	{
		String str("test");
		EXPECT_EQ(str.find("t", 1, 1), 3);
	}
	// rfindのテスト
	{
		String str("test");
		EXPECT_EQ(str.rfind('t'), 3);
	}
	// rfindのテスト(pos)
	{
		String str("test");
		EXPECT_EQ(str.rfind('t', 2), 0);
	}
	// rfindのテスト(String)
	{
		String str("test");
		String str2("t");
		EXPECT_EQ(str.rfind(str2), 3);
	}
	// rfindのテスト(String,pos)
	{
		String str("test");
	}
	// find_first_not_ofのテスト
	{
		String str("test");
		EXPECT_EQ(str.find_first_not_of('t'), 1);
	}
	// find_first_not_ofのテスト(pos)
	{
		String str("test");
		EXPECT_EQ(str.find_first_not_of('t', 2), 2);
	}
	// find_last_not_ofのテスト
	{
		String str("test");
		EXPECT_EQ(str.find_last_not_of('t'), 2);
	}
	// find_last_not_ofのテスト(pos)
	{
		String str("test");
		EXPECT_EQ(str.find_last_not_of('t', 1), 1);
	}
}

TEST(String, Compare) {
	// compareのテスト
	{
		String str("test");
		EXPECT_EQ(str.compare("test"), 0);
	}
	// compareのテスト(String)
	{
		String str("test");
		String str2("test");
		EXPECT_EQ(str.compare(str2), 0);
	}
	// compareのテスト(char*)
	{
		String str("test");
		const char* str2 = "test";
		EXPECT_EQ(str.compare(str2), 0);
	}
	// compareのテスト(StringView)
	{
		String str("test");
		StringView str2("test");
		EXPECT_EQ(str.compare(str2), 0);
	}
	// starts_withのテスト
	{
		String str("test");
		EXPECT_EQ(str.starts_with("t"), true);
	}
	// starts_withのテスト(String)
	{
		String str("test");
		String str2("t");
		EXPECT_EQ(str.starts_with(str2), true);
	}
	// ends_withのテスト
	{
		String str("test");
		EXPECT_EQ(str.ends_with("t"), true);
	}
	// ends_withのテスト(String)
	{
		String str("test");
		String str2("t");
		EXPECT_EQ(str.ends_with(str2), true);
	}
	// containsのテスト
	{
		String str("test");
		EXPECT_EQ(str.contains("t"), true);
	}
	// containsのテスト(String)
	{
		String str("test");
		String str2("t");
		EXPECT_EQ(str.contains(str2), true);
	}
}

TEST(String,CompareOperator) {
	// operator==のテスト
	{
		String str("test");
		EXPECT_EQ(str == "test", true);
	}
	// operator==のテスト(String)
	{
		String str("test");
		String str2("test");
		EXPECT_EQ(str == str2, true);
	}
	// operator==のテスト(char*)
	{
		String str("test");
		const char* str2 = "test";
		EXPECT_EQ(str == str2, true);
	}
	// operator==のテスト(StringView)
	{
		String str("test");
		StringView str2("test");
		EXPECT_EQ(str == str2, true);
	}
	// operator!=のテスト
	{
		String str("test");
		EXPECT_EQ(str != "tset", true);
	}
	// operator!=のテスト(String)
	{
		String str("test");
		String str2("tset");
		EXPECT_EQ(str != str2, true);
	}
	// operator!=のテスト(char*)
	{
		String str("test");
		const char* str2 = "tset";
		EXPECT_EQ(str != str2, true);
	}
	// operator!=のテスト(StringView)
	{
		String str("test");
		StringView str2("tset");
		EXPECT_EQ(str != str2, true);
	}
	// operator<のテスト
	{
		String str("test");
		EXPECT_EQ(str < "tset", true);
	}
	// operator<のテスト(String)
	{
		String str("test");
		String str2("tset");
		EXPECT_EQ(str < str2, true);
	}
	// operator<のテスト(char*)
	{
		String str("test");
		const char* str2 = "tset";
		EXPECT_EQ(str < str2, true);
	}
	// operator<のテスト(StringView)
	{
		String str("test");
		StringView str2("tset");
		EXPECT_EQ(str < str2, true);
	}
	// operator>のテスト
	{
		String str("test");
		EXPECT_EQ(str > "tset", false);
	}
	// operator>のテスト(String)
	{
		String str("test");
		String str2("tset");
		EXPECT_EQ(str > str2, false);
	}
	// operator>のテスト(char*)
	{
		String str("test");
		const char* str2 = "tset";
		EXPECT_EQ(str > str2, false);
	}
	// operator>のテスト(StringView)
	{
		String str("test");
		StringView str2("tset");
		EXPECT_EQ(str > str2, false);
	}
	// operator<=のテスト
	{
		String str("test");
		EXPECT_EQ(str <= "tset", true);
	}
	// operator<=のテスト(String)
	{
		String str("test");
		String str2("tset");
		EXPECT_EQ(str <= str2, true);
	}
	// operator<=のテスト(char*)
	{
		String str("test");
		const char* str2 = "tset";
		EXPECT_EQ(str <= str2, true);
	}
	// operator<=のテスト(StringView)
	{
		String str("test");
		StringView str2("tset");
		EXPECT_EQ(str <= str2, true);
	}
	// operator>=のテスト
	{
		String str("test");
		EXPECT_EQ(str >= "tset", false);
	}
	// operator>=のテスト(String)
	{
		String str("test");
		String str2("tset");
		EXPECT_EQ(str >= str2, false);
	}
	// operator>=のテスト(char*)
	{
		String str("test");
		const char* str2 = "tset";
		EXPECT_EQ(str >= str2, false);
	}
	// operator>=のテスト(StringView)
	{
		String str("test");
		StringView str2("tset");
		EXPECT_EQ(str >= str2, false);
	}
}

TEST(String, OperatorAdd) {
	// operator+のテスト
	{
		String str("test");
		EXPECT_EQ(str + "tset", "testtset");
	}
	// operator+のテスト(String)
	{
		String str("test");
		String str2("tset");
		EXPECT_EQ(str + str2, "testtset");
	}
	// operator+のテスト(char*)
	{
		String str("test");
		const char* str2 = "tset";
		EXPECT_EQ(str + str2, "testtset");
	}
	// operator+のテスト(StringView)
	{
		String str("test");
		StringView str2("tset");
		EXPECT_EQ(str + str2, "testtset");
	}
}
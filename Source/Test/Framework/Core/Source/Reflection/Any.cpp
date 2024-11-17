//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeRegister.h>
#include <Framework/Core/Reflection/TypeInfoPrinter.h>
#include <Framework/Core/Reflection/PrimitiveRegister.h>
#include <nlohmann/json.hpp>

using namespace ob;


struct AnyBar {
	s32 value = 0;
};

struct AnyFoo {
	s32 value = 0;
	AnyBar bar;
	Vector<f32> list;
};

OB_DEFINE_CLASS_INFO(AnyBar) {
	constructor<AnyBar>();
	field("value", & T::value);
}
OB_DEFINE_CLASS_INFO(AnyFoo) {
	constructor();
	field("value", & T::value);
	field("bar", & T::bar);
}

OB_REGISTER_RTTI(AnyBar);
OB_REGISTER_RTTI(AnyFoo);


TEST(Any, Construct) {
	TypeInfoManager manager;

	AnyFoo foo;

	Any a(foo);

	// fooの中身が変わる
	a["value"] = 1;
	EXPECT_EQ(foo.value,1);

	// Propertyは常に参照
	Property property = a["value"];
	property = 2;
	EXPECT_EQ(foo.value, 2);

	// コピーを書き換えてもfooの中身は変わらない
	Any copy = a["value"].copy();
	int value = 3;
	copy = value;
	EXPECT_EQ(foo.value, 2);
	
	// ネストは直接書き込めない
	a["bar"]["value"] = 1;
	EXPECT_EQ(foo.bar.value,0);

	// オブジェクトをコピーして書き込む
	auto bar = a["bar"].copy();
	bar["value"] = 1;
	a["bar"] = bar;

	// 専用のセッターで上記の処理を行える
	//a.set("bar","value", 1);


	// C++20の構文で上記の処理を行える
	// a["bar", "value"] = 1;

	// これはコピーに対する操作
	//a["list"][0] = 1;

	// これはリストの変更
	//auto list = a["list"];
	//list[0] = 1;
	//a["list"] = list;

	// 専用のセッターで上記の処理を行える
	//a.set("list", 0, 1);

	// オブジェクト自体の変更
	a = AnyFoo();
	OB_ASSERT_EXPR(foo.value == 0);

	// 内部オブジェクトに直接アクセス
	a.as<AnyFoo>().value = 1;
	OB_ASSERT_EXPR(foo.value == 1);

	// プロパティのasはコピー
	AnyBar b;
	a["bar"].as<AnyBar>(b);

	Property p = a["bar"];
	p.as<AnyBar>();


	Blob blob;
	BinaryStream stream(blob);
	BinaryReader reader(stream);
	BinaryWriter writer(stream);
	a.seralize(writer);
	a.deserialize(reader);

	sizeof(Delegate<void()>);
	sizeof(Func<void()>);
}
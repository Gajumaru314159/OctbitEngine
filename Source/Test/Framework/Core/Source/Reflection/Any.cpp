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


struct ObjectC {
	s32 _s32 = 0;
};
struct ObjectB {
	s32 _s32 = 0;
	ObjectC _objectC;
};
struct ObjectA {
	bool _bool = 0;
	s32 _s32 = 0;
	f32 _f32 = 0;
	String _string;
	ObjectB _objectB;
};

OB_DEFINE_CLASS_INFO(ObjectC) {
	constructor();
	field("_s32", &T::_s32);
}
OB_DEFINE_CLASS_INFO(ObjectB) {
	constructor();
	field("_s32", &T::_s32);
	field("_objectC", &T::_objectC);
}
OB_DEFINE_CLASS_INFO(ObjectA) {
	constructor();
	field("_bool", &T::_bool);
	field("_s32", &T::_s32);
	field("_f32", &T::_f32);
	field("_string", &T::_string);
	field("_objectB", &T::_objectB);
}
OB_REGISTER_RTTI(ObjectC);
OB_REGISTER_RTTI(ObjectB);
OB_REGISTER_RTTI(ObjectA);





OB_DEFINE_CLASS_INFO(AnyBar) {
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

	// 読み込みはできる
	foo.bar.value = 1;
	auto value2 = a["bar"]["value"].as<s32>();
	EXPECT_EQ(value2, 1);


	// ネスト先に書き込む場合はオブジェクトをコピーして書き込む
	auto bar = a["bar"].copy();
	bar["value"] = 2;
	a["bar"] = bar;

	EXPECT_EQ(foo.bar.value, 2);

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

	auto c = a.copy();
	c["value"] = 2;
	OB_ASSERT_EXPR(foo.value == 1);
	a.assign(c);
	OB_ASSERT_EXPR(foo.value == 2);


	Blob blob;
	BinaryStream stream(blob);
	BinaryReader reader(stream);
	BinaryWriter writer(stream);
	a.seralize(writer);
	a.deserialize(reader);

	sizeof(Delegate<void()>);
	sizeof(Func<void()>);


	ObjectA object1;
	Any obj(object1);
	obj["_bool"] = true;
	obj["_s32"] = 1;
	obj["_f32"] = 2.f;
	obj["_string"] = String("A");
	obj["_objectB"]["_32"] = 3;
	obj["_objectB"]["_objectC"]["_s32"] = 4;

	ObjectA object2;
	object2._bool = true;
	object2._s32 = 1;
	object2._f32 = 2.f;
	object2._string = "A";
	object2._objectB._s32 = 3;
	object2._objectB._objectC._s32 = 4;

}
//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeRegister.h>
using namespace ob;

class Base {

};

class Test : public Base{
public:
	Test() {

	}
	Test(f32 val) {

	}
	void setInt(s32 val){}
	s32 getInt()const {
		return 0;
	}

	void func(s32 a,s32 b) {

	}
};


OB_DEFINE_CLASS_INFO(Base) {
	tag("Description", "ベース");
	constructor();
}

OB_DEFINE_CLASS_INFO(Test) {
	tag("Description", "説明");
	base<Base>();
	ctor();
	constructor();
	constructor<f32>();

	property("Int", &Test::getInt, &Test::setInt).tag("Description", "エー");
}

enum class EnumTest {
	A,
	B,
	C,
};

OB_DEFINE_ENUM_INFO(EnumTest) {
	tag("Description", "説明");
	element("A", ::EnumTest::A).tag("Description","エー");
	element("B", ::EnumTest::B).tag("Description", "ビー");
	element("C", ::EnumTest::C).tag("Description", "シー");
}


OB_REGISTER_RTTI(EnumTest);
OB_REGISTER_RTTI(Test);
OB_REGISTER_RTTI(Base);

namespace a::b::c {
	class AA {

	};
}

TEST(TypeBuilder, Construct) {

	std::array<int, Type::Get<a::b::c::AA>().fullName().size()> a;

	Logger logger;
	
	TypeInfoManager manager;

	LOG_INFO("{}",Type::Get<a::b::c::AA>().name());
	LOG_INFO("{}",Type::Get<a::b::c::AA>().fullName());

	
	manager.visit([](const EnumInfo& info) { LOG_INFO("型：{}", info.type.fullName()); });
	manager.visit([](const ClassInfo& info) { LOG_INFO("型：{}",info.type.fullName()); });

	// TODO 検索
	// TODO 生成
	// TODO デシリアライズ

	if (auto info = manager.findClassInfo(Type("Test"))) {
		LOG_INFO("{}が見つかりました", info->type.fullName());

		for (auto& [name,property] : info->properties) {
		}
		for (auto& function: info->functions) {

		}
		for (auto& constructor : info->converters) {
			// constructor();
		}
	}


}
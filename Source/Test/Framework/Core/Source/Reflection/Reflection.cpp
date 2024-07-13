//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeRegister.h>
using namespace ob;


enum class EnumTest {
	A,
	B,
	C,
};

OB_DEFINE_ENUM_INFO(EnumTest) {
	tag("Description", "説明");
	element("A", T::A).tag("Description", "エー");
	element("B", T::B).tag("Description", "ビー");
	element("C", T::C).tag("Description", "シー");
}

class Base {

};

class TestBase : public Base {
public:
	TestBase() {

	}
	TestBase(f32 val) {
		m_val = val;
	}
	TestBase(s32 val, EnumTest val2) {
		msg = Format("{}/{}", val, enum_cast(val2));
	}
	void setInt(s32 val) {
		m_val = val;
	}
	s32 getInt()const {
		return m_val;
	}
	s32* getIntPtr()const {
		return nullptr;
	}
	void setIntPtr(s32&)const {

	}

	void func(s32 a, s32 b) {

	}
public:
	String msg;
	const String msg2 = "b";
	s32 m_val;
};

class DC {
public:
	DC() {
		LOG_INFO("生成");
		m_value = 0;
	}
	DC(s32 a) {
		LOG_INFO("生成");
		m_value = a;
	}
	DC(s32 a, s32 b) {
		LOG_INFO("生成");
		m_value = a * b;
	}
	~DC() {
		LOG_INFO("破棄");
	}
private:
	s32 m_value;
};


OB_DEFINE_CLASS_INFO(Base) {
	tag("Description", "ベース");
	constructor();
}

OB_DEFINE_CLASS_INFO(TestBase) {
	tag("Description", "説明");
	base<Base>();
	constructor();
	constructor<f32>();
	constructor<s32, EnumTest>("count", "type");

	field("Message", &TestBase::msg);
	field("Message2", &TestBase::msg2);

	property("Int", &T::getInt, &T::setInt).tag("Description", "エー");
	property("ReadOnlyInt", &T::getInt).tag("Description", "エー");
	//property("IntPtr", &T::getIntPtr, &T::setIntPtr).tag("Description", "エー");
}


OB_DEFINE_CLASS_INFO(DC) {
	tag("Description", "ベース");
	constructor<s32 >();
	constructor<s32,s32>();
}



OB_REGISTER_RTTI(EnumTest);
OB_REGISTER_RTTI(TestBase);
OB_REGISTER_RTTI(Base);
OB_REGISTER_RTTI(DC);

namespace a::b::c {
	class AA {

	};
}

TEST(TypeBuilder, Construct) {

	Logger logger;
	TypeInfoManager manager;

	manager.visit([](const TypeInfo& info) {

		// TODO デシリアライズ

		String str;

		str = Format("class {} \n", info.type.shortName());
		if (info.bases.empty() == false) {
			str += "    : ";
			for (auto& base : info.bases) {
				str += Format("public {},\n", base.shortName());
			}
			str.pop_back(2);
			str += "\n";
		}
		str += "{\n";
		str += "public:\n";

		for (auto& constructor : info.constructors) {
			str += Format("    {}(", info.type.shortName());
			for (auto& arg : constructor.arguments) {
				str += Format("{} ", arg.type.name());
				str += Format("{},", arg.name);
			}
			if (constructor.arguments.empty() == false) {
				str.pop_back();
			}
			str += ");\n";
		}

		str += "public:\n";
		for (auto& [name, property] : info.properties) {
			str += Format("    {} {};", property.type.name(), name);
			str += "\n";
		}


		str += "public:\n";
		for (auto& [name, method] : info.methods) {
			str += Format("    {} {}(", method.returnType.name(), method.name);
			for (auto& arg : method.arguments) {
				str += Format("{} ", arg.type.name());
				str += Format("{},", arg.name);
			}
			str.pop_back();
			str += ");\n";
		}
		str += "};";

		LOG_INFO("\n{}", str);
		}
	);

	if (auto info = manager.find("DC")) {
		if (auto ctor = info->findConstructor<>()) {
			auto dc = ctor->invoke<DC>();
			LOG_INFO("==");
		}
		if (auto ctor = info->findConstructor<s32>()) {
			auto dc = ctor->invoke<DC>(3);
			LOG_INFO("==");
		}
		if (auto ctor = info->findConstructor<s32, s32>()) {
			auto dc = ctor->invoke<DC>(3, 4);
			LOG_INFO("==");
		}
	}




	TestBase test(11);
	test.msg = "a";

	const auto& ctest = test;

	if (auto info = manager.find(Type::Get(test))) {
		for (auto& [name, p] : info->properties) {

			if (p.type.is<s32>()) {
				auto value = p.get<s32>(ctest);
				// ImGui::InputInt(name,&value);
				p.set(test, value * value);
			}
			if (p.type.is<f32>()) {
				auto value = p.get<f32>(ctest);

				// valueは内部に参照ポインタかコピーインスタンスを持つ

				// ImGui::InputInt(name,&value);
				p.set(test, value);
			}
			if (p.type.is<String>()) {
				auto value = p.get<String>(ctest);
				p.set(test, value + value);
			}
		}
	}

	LOG_INFO("{}", test.msg);
	LOG_INFO("{}", test.msg2);

}
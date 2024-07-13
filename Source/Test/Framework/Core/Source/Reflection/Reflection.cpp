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

	property("Message", &TestBase::msg);
	property("Message2", &TestBase::msg2);

	property("Int", &T::getInt, &T::setInt).tag("Description", "エー");
	//property("IntPtr", &T::getIntPtr, &T::setIntPtr).tag("Description", "エー");
}


OB_DEFINE_CLASS_INFO(DC) {
	tag("Description", "ベース");
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

	LOG_INFO("{}", Type::Get<a::b::c::AA>().name());
	LOG_INFO("{}", Type::Get<a::b::c::AA>().fullName());

	if (auto info = manager.find(Type("TestBase"))) {
		TestBase base;
		base.setInt(333);

		if (auto itr = info->properties.find("Int"); itr != info->properties.end()) {
			auto& [name, p] = *itr;
			auto result = p.get<s32>(base);
			LOG_INFO("結果：{}", result);
			p.set(base, 222);
			auto result2 = p.get<s32>(base);
			LOG_INFO("結果：{}", result2);

		}
	}

	s32 aval = 123;

	Any empty;
	Any a(123);
	Any b(std::move(aval));
	Any c(aval + 1);

	String abc("abc");
	Any d = abc;
	Any e = String("abc");



	manager.visit([](const TypeInfo& info) {

		// TODO 検索
		// TODO 生成
		// TODO デシリアライズ

		String str;

		str = Format("class {} \n", info.type.name());
		if (info.bases.empty() == false) {
			str += "    : ";
			for (auto& base : info.bases) {
				str += Format("public {},\n", base.name());
			}
			str.pop_back(2);
			str += "\n";
		}
		str += "{\n";
		str += "public:\n";

		for (auto& constructor : info.constructors) {
			str += Format("    {}(", info.type.name());
			for (auto& arg : constructor.arguments) {
				str += Format("{} ", arg.type.fullName());
				str += Format("{},", arg.name);
			}
			if (constructor.arguments.empty() == false) {
				str.pop_back();
			}
			str += ");\n";
		}

		str += "public:\n";
		for (auto& [name, property] : info.properties) {
			str += Format("    {} {};", property.type.fullName(), name);
			str += "\n";
		}


		str += "public:\n";
		for (auto& [name, method] : info.methods) {
			str += Format("    {} {}(", method.returnType.fullName(), method.name);
			for (auto& arg : method.arguments) {
				str += Format("{} ", arg.type.fullName());
				str += Format("{},", arg.name);
			}
			str.pop_back();
			str += ");\n";
		}
		str += "};";

		LOG_INFO("\n{}", str);
		});

	if (auto info = manager.find("DC")) {
		if (auto ctor = info->findConstructor<s32, s32>()) {
			ConstAnyReference args[]{
				3,
				4
			};
			for (auto& arg : args) {
				LOG_INFO("{}",arg.type().name());
			}
			auto dc = ctor->invoke<DC>(args);
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
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

class TestBase : public Base{
public:
	TestBase() {

	}
	TestBase(f32 val) {

	}
	TestBase(s32 val,EnumTest val2) {
		msg = Format("{}/{}",val,enum_cast(val2));
	}
	void setInt(s32 val){
		m_val = val;
	}
	s32 getInt()const {
		return m_val;
	}

	void func(s32 a,s32 b) {

	}
public:
	String msg;
	s32 m_val;
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
	constructor<s32,EnumTest>("count","type");

	//property("Message", &TestBase::msg);

	property("Int", &T::getInt, &T::setInt).tag("Description", "エー");
}



OB_REGISTER_RTTI(EnumTest);
OB_REGISTER_RTTI(TestBase);
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

	if (auto info = manager.find(Type("TestBase"))) {
		TestBase base;
		base.setInt(333);

		if (auto itr = info->properties.find("Int"); itr != info->properties.end()) {
			auto result = itr->second.getter(&base);
			auto re = reinterpret_cast<const s32*>(result.pointer);
			LOG_INFO("{}",*re);
		}
	}

	
	manager.visit([](const TypeInfo& info) { 

		// TODO 検索
		// TODO 生成
		// TODO デシリアライズ

		String str;
		
		str = Format("class {} \n", info.type.name());
		if (info.bases.empty() == false) {
			str += "    : ";
			for (auto& base : info.bases) {
				str += Format("public * {},\n", base.name());
			}
			str.pop_back();
		}
		str += " {\n";
		str += "public:\n";

		for (auto& constructor : info.constructors) {
			str += Format("    {}(", info.type.name());
			for (auto& arg : constructor.arguments) {
				str += Format("{} ", arg.type.fullName());
				str += Format("{},",arg.name);
			}
			if (constructor.arguments.empty() == false) {
				str.pop_back();
			}
			str += ");\n";
		}

		str += "public:\n";
		for (auto& [name, property] : info.properties) {
			str += Format("    {} {};",property.type.fullName(),name);
			str += "\n";
		}


		str += "public:\n";
		for (auto& [name,method] : info.methods) {
			str += Format("    {} {}(",method.returnType.fullName(),method.name);
			for (auto& arg : method.arguments) {
				str += Format("{} ", arg.type.fullName());
				str += Format("{},", arg.name);
			}
			str.pop_back();
			str += ");\n";
		}
		str += "};";

		LOG_INFO("\n{}",str);
	});

	class Component {
	public:
		Type getType() { return Type::Get<s32>(); }
	} component;

	if (auto info = manager.find(component.getType())) {
		for (auto& [name, p] : info->properties) {

			if (p.type.is<s32>()) {
				auto value = p.get<s32>(&component);
				// ImGui::InputInt(name,&value);
				p.set(component, value);
			}
			if (p.type.is<f32>()) {
				auto value = p.get<f32>(&component);

				// valueは内部に参照ポインタかコピーインスタンスを持つ

				// ImGui::InputInt(name,&value);
				p.set(component, value);
			}
		}
	}

}
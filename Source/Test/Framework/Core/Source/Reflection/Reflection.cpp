//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeRegister.h>
#include <Framework/Core/Reflection/TypeInfoPrinter.h>
#include <nlohmann/json.hpp>

using namespace ob;

enum class FruitType : u32 {
	Apple,
	Melon,
	Lemon,
};

class Food {
public:
	virtual ~Food() = default;
};

struct Nutrients {
	f32 calorie;
	f32 fat;
	f32 sugar;
};

class Fruit : public Food {
public:
	Fruit() : Fruit(FruitType::Apple, 0, 0) {
	}
	Fruit(FruitType type, s32 price, f32 weight) {
		m_type = type;
		m_price = price;
		m_weight = weight;

		m_nutrients.calorie = 64;
		m_nutrients.fat = 0;
		m_nutrients.sugar = 100;
	}

	~Fruit() {
		LOG_INFO("破棄");
	}

	Map<s32,String> getPriceHistory()const { 
		return { {1,"A"}, {2,"B"}, {3,"C"}};
	}

	FruitType getType()const { return m_type; }
	void setType(FruitType type){ m_type = type; }

	void setPrice(s32 price) { m_price = price; }
	auto getPrice()const { return m_price; }


	const Nutrients& getNutrients() const { return m_nutrients; }

	void print(bool console, bool file)const {
		// プリント処理
	}

	String toString()const {
		return Format("{}:{}円", enum_cast(m_type), m_price);
	}
private:
	Nutrients m_nutrients;
	FruitType m_type;
	s32 m_price;
public:
	f32 m_weight;
};

OB_DEFINE_ENUM_INFO(FruitType) {
	tag("Description", "フルーツの種類");
	element("Apple", T::Apple).desc("リンゴ");
	element("Melon", T::Melon).desc("メロン");
	element("Lemon", T::Lemon).desc("レモン");
}

OB_DEFINE_CLASS_INFO(Nutrients) {
	tag("Description", "栄養素");
	field("Calorie", &T::calorie);
	field("Fat", &T::fat);
	field("Sugar", &T::sugar);
}

OB_DEFINE_CLASS_INFO(Food) {
	tag("Description", "食べ物");
}
OB_DEFINE_CLASS_INFO(Fruit) {
	tag("Description", "Fruitフルーツ");
	base<Food>();
	constructor().desc("デフォルトコンストラクタ");
	constructor<FruitType, s32, f32>("type", "price", "weight").desc("プロパティを指定して生成");
	method("print", &T::print, "console", "file");
	method("toString", &T::toString);
	property("PriceHistory", &T::getPriceHistory);
	property("Nutrients", &T::getNutrients);
	property("Type", &T::getType,&T::setType);
	property("Price", &T::getPrice, &T::setPrice);
	field("Weight", &T::m_weight);
}

OB_REGISTER_RTTI(Nutrients);
OB_REGISTER_RTTI(FruitType);
OB_REGISTER_RTTI(Food);
OB_REGISTER_RTTI(Fruit);


using Vector_int = Vector<int>;
OB_DEFINE_CLASS_INFO(Vector_int) {
	constructor();
	property("size", &T::size);
	property("max_size", &T::max_size);
	method<void, size_t>("resize", &T::resize, "size");
	property("capacity", &T::capacity);
	property("empty", &T::empty);
	method("reserve", &T::reserve, "n");
	method("shrink_to_fit", &T::shrink_to_fit);

	method<int&, size_t>("at", &T::at, "n");

	method<void, size_t, const int&>("assign", &T::assign, "n", "t");
	method<void, const int&>("push_back", &T::push_back, "x");
	method("pop_back", &T::pop_back);
	method("clear", &T::clear);
}
OB_REGISTER_RTTI(Vector_int);

TEST(TypeBuilder, Construct) {
	TypeInfoManager manager;

	EXPECT_NE(manager.find("Nutrients"), nullptr);
	EXPECT_NE(manager.find("FruitType"), nullptr);
	EXPECT_NE(manager.find("Food"), nullptr);
	EXPECT_NE(manager.find("Fruit"), nullptr);
	EXPECT_NE(manager.find(Type::Get<Nutrients>()), nullptr);
	EXPECT_NE(manager.find(Type::Get<FruitType>()), nullptr);
	EXPECT_NE(manager.find(Type::Get<Food>()), nullptr);
	EXPECT_NE(manager.find(Type::Get<Fruit>()), nullptr);

	auto items = Type::Get<Vector<int>>();
	if (auto info = TypeInfo::Find(items)) {

		{
			Vector<int> items3;
			Any items2(items3);

			if (auto func = info->findMethod("resize")) {
				Any args[]{ 3ull };
				func->invoke(items2, args);
			
			}

			if (auto prop = info->findProperty("size")) {
				size_t size = prop->get<size_t>(items2);
				EXPECT_EQ(size, 3);
			}
			items3.resize(199);

			if (auto prop = info->findProperty("size")) {
				size_t size = prop->get<size_t>(items2);
				EXPECT_EQ(size, 199);
			}
		}



		if (auto ctor = info->findConstructor()) {
			auto items2 = ctor->invoker({});
			
			if (auto func = info->findMethod("resize")) {
				Any args[]{ 3ull };
				func->invoke(items2, args);

			}

			if (auto prop = info->findProperty("size")) {
				size_t size = prop->get<size_t>(items2);
			}

		}
	}

}


class MethodTest {
public:

	MethodTest() = default;

	void void_no_args() {

	}
	void void_no_args_const() const {

	}
	void void_no_args_noexcept() noexcept{

	}
	void void_no_args_const_noexcept() const noexcept{

	}


	int int_no_args() {
		return 1;
	}
	int int_no_args_const() const {
		return 1;
	}
	int int_no_args_noexcept() noexcept {
		return 1;
	}
	int int_no_args_const_noexcept() const noexcept {
		return 1;
	}

	void void_args(int,float) {

	}
	void void_args_const(int, float) const {

	}
	void void_args_noexcept(int, float) noexcept {

	}
	void void_args_const_noexcept(int, float) const noexcept {

	}


	int int_args(int, float) {
		return 1;
	}
	int int_args_const(int, float) const {
		return 1;
	}
	int int_args_noexcept(int, float) noexcept {
		return 1;
	}
	int int_args_const_noexcept(int, float) const noexcept {
		return 1;
	}

};
OB_DEFINE_CLASS_INFO(MethodTest) {
	constructor();

	method("void_no_args", &T::void_no_args);
	method("void_no_args_const", &T::void_no_args_const);
	method("void_no_args_noexcept", &T::void_no_args_noexcept);
	method("void_no_args_const_noexcept", &T::void_no_args_const_noexcept);

	method("int_no_args", &T::int_no_args);
	method("int_no_args_const", &T::int_no_args_const);
	method("int_no_args_noexcept", &T::int_no_args_noexcept);
	method("int_no_args_const_noexcept", &T::int_no_args_const_noexcept);

	method("void_args", &T::void_args, "a", "b");
	method("void_args_const", &T::void_args_const, "a", "b");
	method("void_args_noexcept", &T::void_args_noexcept, "a", "b");
	method("void_args_const_noexcept", &T::void_args_const_noexcept, "a", "b");

	method("int_args", &T::int_args, "a", "b");
	method("int_args_const", &T::int_args_const, "a", "b");
	method("int_args_noexcept", &T::int_args_noexcept, "a", "b");
	method("int_args_const_noexcept", &T::int_args_const_noexcept, "a", "b");
}
OB_REGISTER_RTTI(MethodTest);



class PropertyTest {
public:

	PropertyTest() = default;

	int get() { return 1; }
	int get_const()const { return 1; }
	int get_noexceot()noexcept { return 1; }
	int get_const_noexcept()const noexcept { return 1; }
	void set(s32) { }
	void set_noexceot(s32)noexcept { }

};
OB_DEFINE_CLASS_INFO(PropertyTest) {
	constructor();

	property("get", &T::get);
	property("get_const", &T::get_const);
	property("get_noexcept", &T::get_noexceot);
	property("get_const_noexcept", &T::get_const_noexcept);

	property("get_set", &T::get, &T::set);
	property("get_set_noexcept", &T::get_noexceot, &T::set_noexceot);
}
OB_REGISTER_RTTI(PropertyTest);

struct FieldTest {
	int value;
	int& ref;
	const int const_value;
	const int& const_reference;
};
OB_DEFINE_CLASS_INFO(FieldTest) {

	field("value", &T::value);
	//field("ref", &T::ref);
	//field("const_value", &T::const_value);
	//field("const_reference", &T::const_reference);
}
OB_REGISTER_RTTI(FieldTest);
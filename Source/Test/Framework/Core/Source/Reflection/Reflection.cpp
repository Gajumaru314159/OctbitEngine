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

}
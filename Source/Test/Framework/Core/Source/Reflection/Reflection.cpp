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


nlohmann::json Serealize(const AnyReference& owner,const TypeInfoManager& manager) {
	nlohmann::json obj;
	auto& type = owner.type();

	if (false);
	else if (type.is<s32>()) obj = owner.get<const s32>();
	else if (type.is<f32>()) obj = owner.get<const f32>();
	else if (type.is<String>()) obj = owner.get<const String>();
	else if (owner.list()) {
		for (auto element : owner.list()) {
			obj.emplace_back(Serealize(element,manager));
		}
	}
	else if (owner.map()) {
		for (auto [key,value] : owner.map()) {
			auto& item = obj.emplace_back();
			item["Key"] = Serealize(key, manager);
			item["Value"] = Serealize(value, manager);
		}
	}
	else if (auto info = manager.find(type)) {
		if (info->isEnum) {
			if (info->enumValueGetter) {
				auto value = info->enumValueGetter(owner);
				if (auto enumInfo = info->findEnumElement(value)) {
					obj = enumInfo->name;
				}
			}
		}
		else {
			obj["@Type"] = type.name();
			for (auto& [name, p] : info->properties) {
				if (!p.canRead())continue;
				obj[name] = Serealize(p.getter(owner), manager);
			}
		}
	}

	return obj;
}


Any Deserealize(const nlohmann::json& obj , const TypeInfoManager& manager) {

	if (!obj.contains("@Type"))return {};
	auto type = obj["@Type"].operator std::string();
	
	if (auto info = manager.find(type)) {

		if (auto ctor = info->findConstructor()) {

			// インスタンス生成
			auto instance = ctor->invoker({});
			AnyReference owner = instance;

			for (auto& [name, property] : info->properties) {
				if (!property.canWrite()) continue;

				auto itr = obj.find(name);
				if (itr == obj.end())continue;
				auto& value = *itr;

				auto& type = property.type;

				if (false);
				else if (type.is<s32>()) property.set(owner, value.operator s32());
				else if (type.is<f32>()) property.set(owner, value.operator f32());
				else if (type.is<String>()) property.set(owner, value.operator String());
				else if (owner.list()) {
					for (auto& item : value.array()) {
						// 要素型が必要
						// Deserealize(item,manager);
					}
				}
				else if (owner.map()) {
					for (auto& [key,value] : value.items()) {
						// キーと要素型が必要
						// Deserealize(key, manager);
						// Deserealize(value, manager);
					}
				}
				else if (auto info = manager.find(type)) {
					if (info->isEnum) {
						if (auto enumInfo = info->findEnumElement(value.operator String())) {
							property.setter(owner, enumInfo->sample);
						}						
					}
					else {
						if (Any userObject = Deserealize(*itr, manager)) {
							property.setter(owner, userObject);
						}
					}
				}
			}

			return std::move(instance);
		}

	}
	return {};
}

TEST(TypeBuilder, Construct) {

	Logger logger;
	TypeInfoManager manager;

	AnyReference::callable_get_type<FruitType&>::value;

	manager.visit(
		[](const TypeInfo& info) {
			LOG_INFO("\n{}", PrintTypeInfo(info));
		}
	);

	Fruit fruit(FruitType::Lemon, 1111, 3.14f);

	if (auto info = manager.find(Type::Get(fruit))) {

		nlohmann::json clazz = Serealize(fruit, manager);

		LOG_INFO("\n{}",clazz.dump(4));

		if (auto copy = Deserealize(clazz, manager)) {
			auto& copyFruit = copy.get<Fruit>();
			LOG_INFO("{}", copyFruit.getPrice());


			nlohmann::json clazz2 = Serealize(copyFruit, manager);

			LOG_INFO("\n{}", clazz2.dump(4));
		}
	}

}
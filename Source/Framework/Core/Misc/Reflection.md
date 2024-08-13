Reflection
==========

## クラス一覧
* 型情報
    * TagInfo
	* EnumElementInfo
	* ArgumentInfo
	* ConstructorInfo
	* PropertyInfo
	* MethodInfo
	* TypeInfo
* ビルダー
    * TagBuilder
	* EnumBuilder
	* ClassBuilder
	* EnumBuilderTemplate
	* ClassBuilderTemplate
* マネージャー
    * TypeInfoManager
* インスタンス
    * Any
	* AnyReference

## 型情報の登録

### クラス型情報の登録

`OB_DEFINE_CLASS_INFO`は、クラスの型情報を登録するためのマクロです。  
このマクロを使用することで、クラスのメンバー変数、メソッド、プロパティ、コンストラクタなどの情報を登録することができます。

```c++
class Fruit : public Food {
public:
	Fruit();
	Fruit(FruitType type, s32 price, f32 weight);
	void setPrice(s32 price);
	auto getPrice()const -> s32;
	void print(bool console, bool file)const;
	auto toString()const->String;
public:
	f32 m_weight;
};
```
```c++
OB_DEFINE_CLASS_INFO(Fruit) {
	tag("Description", "Fruitフルーツ");
	base<Food>();
	constructor().desc("デフォルトコンストラクタ");
	constructor<FruitType, s32, f32>("type", "price", "weight").desc("プロパティを指定して生成");
	method("print", &T::print, "console", "file");
	method("toString", &T::toString);
	property("Price", &T::getPrice, &T::setPrice);
	field("Weight", &T::m_weight);
}
```

### 列挙型情報の登録
列挙型の情報を登録するためには、`OB_DEFINE_ENUM_INFO`マクロを使用します。  
`element()`の第一引数には列挙子の名前、第二引数には列挙子の値を指定します。
必要に応じてチェーンメソッドの`desc()`を使用して説明を追加することができます。
その他任意のタグを追加したい場合はチェーンメソッドの`tag()`を使用してください。
```c++
enum class FruitType : u32 {
	Apple,
	Melon,
	Lemon,
};
```
```c++
OB_DEFINE_ENUM_INFO(FruitType) {
	tag("Description", "フルーツの種類");
	element("Apple", T::Apple).desc("リンゴ");
	element("Melon", T::Melon).desc("メロン");
	element("Lemon", T::Lemon).desc("レモン").tag("Color","#FFFF00");
}
```
クラス型情報と同様に`OB_REGISTER_RTTI`を使用してマネージャーに型情報を登録してください。
```c++
OB_REGISTER_RTTI(FruitType);
```

## 型情報の取得
型情報はTypeInfoManagerから取得できます。  
```c++
if(auto info = TypeInfoManager::Find(Type::Get<Fruit>())){

}
if(auto info = TypeInfoManager::Find("Fruit")){

}
```

## インスタンスの生成
インスタンスの生成をするためにはTypeInfoからConstructorInfoを探す必要があります。
`findConstructor`を使用して呼出し可能なコンストラクタ―を取得してください。
`ConstructorInfo::invoke`を引数とともに呼び出すことでインスタンスを生成することができます。
```c++
if(auto info = TypeInfoManager::Find("Fruit")){
	if(auto ctor = info->findConstructor<FruitType,s32,f32>()){
		auto fruit = ctor->invoke<Fruit>(FruitType::Lemon,100,1.0f);
	}
}
```
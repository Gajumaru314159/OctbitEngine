//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Engine/Reflection/TypeInfo.h>


//@―---------------------------------------------------------------------------
//! @brief		Enum型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_ENUM_TYPE_INFO(type)\
namespace type_info_builder::type {\
	void Link() {\
		type_info_builder::EnumBuilderTemplate<::type>{};\
	}\
}\
template<> void type_info_builder::EnumBuilderTemplate<::type>::Register()


//@―---------------------------------------------------------------------------
//! @brief		Class型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_CLASS_TYPE_INFO(type)\
namespace type_info_builder::type {\
	void Link() {\
		type_info_builder::ClassBuilderTemplate<::type>{};\
	}\
}\
template<> void type_info_builder::ClassBuilderTemplate<::type>::Register()


namespace ob::engine::rtti::internal {

	//@―---------------------------------------------------------------------------
	//! @brief		Enum要素情報ビルダー
	//@―---------------------------------------------------------------------------
	class ElementBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		ElementBuilder(ElementInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief		タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = TC(""));

	private:
		ElementInfo& m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報ビルダー
	//@―---------------------------------------------------------------------------
	class EnumBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		EnumBuilder(EnumInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief		タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = TC(""));

		//@―---------------------------------------------------------------------------
		//! @brief		要素追加
		//! @details	追加した順番にインデックスが割り振られます。インデックスは0ベースです。
		//@―---------------------------------------------------------------------------
		template<typename E>
		ElementBuilder element(StringView name, E value) {
			return ElementBuilder(elementImpl(name, enum_cast(value)));
		}

	private:
		ElementInfo& elementImpl(StringView name, s64 value);
	private:
		EnumInfo& m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		プロパティ情報ビルダー
	//@―---------------------------------------------------------------------------
	class PropertyBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		PropertyBuilder(PropertyInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief		タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = TC(""));

	private:
		PropertyInfo& m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		関数情報ビルダー
	//@―---------------------------------------------------------------------------
	class FunctionBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		FunctionBuilder(FunctionInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief		タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = TC(""));

	private:
		FunctionInfo& m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		クラス情報ビルダー
	//@―---------------------------------------------------------------------------
	class ClassBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//@―---------------------------------------------------------------------------
		ClassBuilder(ClassInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief			タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = TC(""));


		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ追加
		//! @tparam Args	引数型リスト
		//@―---------------------------------------------------------------------------
		//template<class... Args>
		//void constructor() {
		//	constructorImpl();
		//}

		//@―---------------------------------------------------------------------------
		//! @brief			関数追加
		//@―---------------------------------------------------------------------------
		template<class TFunc>
		FunctionBuilder function(StringView name, TFunc function) {
			return functionImpl(name);
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(メンバ変数)
		//@―---------------------------------------------------------------------------
		template<class TClass, class TField>
		PropertyBuilder& property(StringView name, TField TClass::* address,bool writable = true) {
			PropertyInfo p; 
			p.name = name;
			m_info.m_properties.emplace(p);
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F>
		PropertyBuilder property(StringView name, F getter) {
			PropertyInfo p;
			p.name = name;
			m_info.m_properties.emplace(p);
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F1, class F2>
		PropertyBuilder property(StringView name, F1 getter, F2 setter) {

			PropertyInfo p;
			p.name = name;
			m_info.m_properties.emplace(p);
		}

		// 以下バージョン互換機能
		
		//@―---------------------------------------------------------------------------
		//! @brief			バージョン設定
		//@―---------------------------------------------------------------------------
		void version(s32 version/*,VersionConverter converter = nullptr*/);

		//@―---------------------------------------------------------------------------
		//! @brief			型変更
		//@―---------------------------------------------------------------------------
		//template <class TFrom, class TTo>
		//ClassBuilder& typeChange(StringView fieldName, s32 fromVersion, s32 toVersion, Func<TTo(const TFrom&)> upgradeFunc);

		//@―---------------------------------------------------------------------------
		//! @brief			名前変更
		//@―---------------------------------------------------------------------------
		void nameChange(s32 fromVersion, s32 toVersion, StringView oldName, StringView newName);


	private:

		void baseImpl(TypeId);
		void constructorImpl(TypeId);
		FunctionBuilder functionImpl(StringView name);

	private:
		ClassInfo& m_info;
	};

}



namespace type_info_builder {

	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報生成
	//@―---------------------------------------------------------------------------
	::ob::engine::rtti::EnumInfo& CreateEnumInfo(::ob::TypeId);

	//@―---------------------------------------------------------------------------
	//! @brief		Class型情報生成
	//@―---------------------------------------------------------------------------
	::ob::engine::rtti::ClassInfo& CreateClassInfo(::ob::TypeId);



	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class EnumBuilderTemplate :public ::ob::engine::rtti::internal::EnumBuilder {
	public:
		EnumBuilderTemplate() : ::ob::engine::rtti::internal::EnumBuilder(CreateEnumInfo(::ob::TypeId::Get<T>())) {}
		void Register() {}
	};

	//@―---------------------------------------------------------------------------
	//! @brief		Class型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class ClassBuilderTemplate :public ::ob::engine::rtti::internal::ClassBuilder {
	public:
		ClassBuilderTemplate() : ::ob::engine::rtti::internal::ClassBuilder(CreateClassInfo(::ob::TypeId::Get<T>())) {}

		template<class TBase,class = std::enable_if_t<std::is_base_of_v<TBase,T>>>
		void base() { baseImpl(::ob::TypeId::Get<TBase>()); }
		void Register() {}
	};

}
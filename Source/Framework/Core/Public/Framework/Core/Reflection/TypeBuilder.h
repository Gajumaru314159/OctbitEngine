//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>

namespace type_info_builder {

	struct ReflectionFunction {
		using func_type = void(*)();
		ReflectionFunction(func_type func):func(func){}
		func_type func;
		ReflectionFunction* next = nullptr;
	};
	ReflectionFunction* GetReflectionFunction();
	void PushReflectionFunction(ReflectionFunction*);

}


//@―---------------------------------------------------------------------------
//! @brief		Enum型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_ENUM_INFO(type)\
namespace type_info_builder::type {\
	void Register() {\
		ob::core::internal::EnumBuilderTemplate<::type> builder{};\
	}\
}\
template<> void ob::core::internal::EnumBuilderTemplate<::type>::Register()


//@―---------------------------------------------------------------------------
//! @brief		Class型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_CLASS_INFO(type)\
namespace type_info_builder::type {\
	void Register() {\
		ob::core::internal::ClassBuilderTemplate<::type> builder{};\
	}\
}\
template<> void ob::core::internal::ClassBuilderTemplate<::type>::Register()


namespace ob::core::internal {

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
		void tag(StringView key, StringView value = "");

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
		void tag(StringView key, StringView value = "");

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
		PropertyBuilder(PropertyInfo*);

		//@―---------------------------------------------------------------------------
		//! @brief		タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = "");

	private:
		PropertyInfo* m_info;
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
		void tag(StringView key, StringView value = "");

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
		void tag(StringView key, StringView value = "");


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
		PropertyBuilder property(StringView name, TField TClass::* address,bool writable = true) {
			auto& info = m_info.properties[name];
			info.name = name;
			return PropertyBuilder(&info);
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F>
		PropertyBuilder property(StringView name, F getter) {
			auto& info = m_info.properties[name];
			info.name = name;
			return PropertyBuilder(&info);
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F1, class F2>
		PropertyBuilder property(StringView name, F1 getter, F2 setter) {
			auto& info = m_info.properties[name];
			info.name = name;
			info.setter = [](void* owner, const void* value) {  };
			return PropertyBuilder(&info);
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
		void convert(s32 fromVersion, s32 toVersion, StringView oldName, StringView newName) {
			auto& converter = m_info.converters.emplace_back();
			converter.fromVersion = fromVersion;
			converter.toVersion = toVersion;
			converter.from = oldName;
			converter.to = newName;
		}


	protected:

		void baseImpl(TypeId);
		void constructorImpl();
		PropertyBuilder addPropertyImpl() {}
		FunctionBuilder functionImpl(StringView name);

	private:
		ClassInfo& m_info;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報生成
	//@―---------------------------------------------------------------------------
	EnumInfo& CreateEnumInfo(ob::TypeId);

	//@―---------------------------------------------------------------------------
	//! @brief		Class型情報生成
	//@―---------------------------------------------------------------------------
	ClassInfo& CreateClassInfo(ob::TypeId);



	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class EnumBuilderTemplate :public EnumBuilder {
	public:
		EnumBuilderTemplate() : EnumBuilder(TypeInfoManager::Instance().registerEnumInfo(TypeId::Get<T>())) {
			Register();
		}
		void Register();
	};

	//@―---------------------------------------------------------------------------
	//! @brief		Class型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class ClassBuilderTemplate : public ClassBuilder {
	public:
		ClassBuilderTemplate() : ClassBuilder(TypeInfoManager::Instance().registerClassInfo(TypeId::Get<T>())) {
			Register();
		}
		void Register();

		template<class TBase,class = std::enable_if_t<std::is_base_of_v<TBase,T>>>
		void base() { baseImpl(::ob::TypeId::Get<TBase>()); }


		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ追加
		//! @tparam Args	引数型リスト
		//@―---------------------------------------------------------------------------
		void ctor() {
			auto creator = []() { return new T(); };
		}

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ追加
		//! @tparam Args	引数型リスト
		//@―---------------------------------------------------------------------------
		template<class... Args>
		void constructor() {

			// {TypeId::Get<Args>()...}

			// new T(Args{args});

			constructorImpl();
		}
	};

}
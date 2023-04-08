//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::engine::rtti {

	namespace internal {
		class ClassBuilder;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  タグ情報
	//@―---------------------------------------------------------------------------
	struct TaggedInfo {
		Map<String, String, std::less<>> tags;

		//@―---------------------------------------------------------------------------
		//! @brief		タグを持っているか
		//@―---------------------------------------------------------------------------
		bool hasTag(StringView name)const { return tags.count(name); }

		//@―---------------------------------------------------------------------------
		//! @brief		タグを取得
		//@―---------------------------------------------------------------------------
		Optional<StringView> findTag(StringView name)const { auto found = tags.find(name); return (found == tags.end()) ? std::nullopt : Optional<StringView>{ found->second }; }
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Enum要素情報
	//@―---------------------------------------------------------------------------
	struct ElementInfo:TaggedInfo {
		String	name;
		s32		index;
		s64		value;

		//@―---------------------------------------------------------------------------
		//! @brief  Enum型を指定して取得
		//@―---------------------------------------------------------------------------
		template<class T, class = std::enable_if_t<std::is_enum_v<T>>>
		T get()const noexcept {
			return static_cast<T>(value);
		}
	};



	//@―---------------------------------------------------------------------------
	//! @brief  Enum型情報
	//@―---------------------------------------------------------------------------
	struct EnumInfo:TaggedInfo {
		Array<ElementInfo> elements;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  プロパティ情報
	//@―---------------------------------------------------------------------------
	struct PropertyInfo:TaggedInfo {

		using Setter = Func<void(void*, const void*)>;
		using Getter = Func<const void*(void*)>;

		String name;
		TypeId type;
		Getter getter;
		Setter setter;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  関数情報
	//@―---------------------------------------------------------------------------
	struct FunctionInfo:TaggedInfo {
	public:
		String name;
		//Func
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Class型情報
	//@―---------------------------------------------------------------------------
	class ClassInfo :public TaggedInfo {
		friend class ob::engine::rtti::internal::ClassBuilder;
	public:

		// 名前で検索できるようにする
		struct PropertyInfoPred {
			bool operator()(const PropertyInfo& left, const PropertyInfo& right) const noexcept {
				return left.name < right.name;
			}
			bool operator()(const PropertyInfo& left, StringView right) const noexcept {
				return left.name < right;
			}
		};

		using PropertyInfoSet	= Set<PropertyInfo, PropertyInfoPred>;
		using FunctionInfoSet	= MultiMap<String,FunctionInfo, std::less<>>;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		基底クラスを取得
		//! @detaisl	プリミティブ型はnullptrを返す。
		//@―---------------------------------------------------------------------------
		ClassInfo* base()const;

		//@―---------------------------------------------------------------------------
		//! @brief		継承しているか
		//@―---------------------------------------------------------------------------
		bool inherit(ClassInfo*)const;

		//@―---------------------------------------------------------------------------
		//! @brief		型のハッシュ値を返す
		//! @details	プラットフォームによらず固定です。
		//! @note		内部実装は型名のハッシュ値です。
		//@―---------------------------------------------------------------------------
		//TypeId hash()const { return m_typeId; }


		const PropertyInfoSet& properties()const { return m_properties; }

	private:

		HashSet<TypeId> m_baseClasses;
		PropertyInfoSet m_properties;
		FunctionInfoSet m_functions;

	};


}// namespcae ob

//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <Framework/Core/Template/Container/Map.h>

namespace ob::core {

	namespace internal {
		class ClassBuilder;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  タグ情報
	//@―---------------------------------------------------------------------------
	struct TaggedInfo {

		HashMap<StringView, String> tags;

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
	struct ElementInfo : TaggedInfo {
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
	struct EnumInfo :TaggedInfo {
		TypeId				typeId;
		Vector<ElementInfo> elements;
	};


	struct PropertyConverter {
		s32 fromVersion;
		s32 toVersion;
		StringView from;
		StringView to;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  プロパティ情報
	//@―---------------------------------------------------------------------------
	struct PropertyInfo :TaggedInfo {

		using Setter = Func<void(void*, const void*)>;
		using Getter = Func<const void* (void*)>;

		TypeId typeId;
		String name;
		Getter getter;
		Setter setter;

	};


	//@―---------------------------------------------------------------------------
	//! @brief  関数情報
	//@―---------------------------------------------------------------------------
	struct FunctionInfo :TaggedInfo {
	public:
		String name;
		//Func
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Class型情報
	//@―---------------------------------------------------------------------------
	struct ClassInfo : TaggedInfo {

		using PropertyInfoMap = HashMap<StringView,PropertyInfo>;
		using FunctionInfoSet = MultiMap<String, FunctionInfo, std::less<>>;
	
		TypeId			typeId;
		HashSet<TypeId> bases;
		PropertyInfoMap properties;
		FunctionInfoSet functions;

		HashSet<TypeId> derivedes;


		Vector<PropertyConverter> converters;
	};


}

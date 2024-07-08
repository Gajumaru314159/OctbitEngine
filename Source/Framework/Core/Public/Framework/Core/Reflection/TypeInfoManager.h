//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Template/Container/Map.h>
#include <Framework/Core/Template/Container/Set.h>
#include <Framework/Core/Utility/Singleton.h>
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Reflection/TypeInfo.h>

namespace ob::core {

	class TypeInfoManager : public Singleton<TypeInfoManager> {
	public:

		TypeInfoManager();

		void visit(const std::function<void(const EnumInfo&)> func) {
			for (auto& [type, info] : m_enums) func(info);
		}
		void visit(const std::function<void(const ClassInfo&)> func) {
			for (auto& [type, info] : m_classes) func(info);
		}

		const EnumInfo* findEnumInfo(Type type)const {
			if (auto itr = m_enums.find(type); itr != m_enums.end()) {
				return &itr->second;
			}
			return nullptr;
		}
		const ClassInfo* findClassInfo(Type type)const {
			if (auto itr = m_classes.find(type); itr != m_classes.end()) {
				return &itr->second;
			}
			return nullptr;
		}


		EnumInfo& registerEnumInfo(Type type) {
			auto& info = m_enums[type];
			info.type = type;
			return info;
		}
		ClassInfo& registerClassInfo(Type type) {
			auto& info = m_classes[type];
			info.type = type;
			return info;
		}

	private:
		HashMap<Type, EnumInfo> m_enums;
		HashMap<Type, ClassInfo> m_classes;
		//HashMap<Type, PrimitiveInfo> m_primitives;


	};
}
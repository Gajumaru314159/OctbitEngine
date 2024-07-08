//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Template/Container/Map.h>
#include <Framework/Core/Template/Container/Set.h>
#include <Framework/Core/Utility/Singleton.h>
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Core/Reflection/TypeInfo.h>

namespace ob::core {

	class TypeInfoManager : public Singleton<TypeInfoManager> {
	public:

		TypeInfoManager();

		void visit(const std::function<void(const EnumInfo&)> func) {
			for (auto& [typeId, info] : m_enums) func(info);
		}
		void visit(const std::function<void(const ClassInfo&)> func) {
			for (auto& [typeId, info] : m_classes) func(info);
		}

		const EnumInfo* findEnumInfo(TypeId typeId)const {
			if (auto itr = m_enums.find(typeId); itr != m_enums.end()) {
				return &itr->second;
			}
			return nullptr;
		}
		const ClassInfo* findClassInfo(TypeId typeId)const {
			if (auto itr = m_classes.find(typeId); itr != m_classes.end()) {
				return &itr->second;
			}
			return nullptr;
		}


		EnumInfo& registerEnumInfo(TypeId typeId) {
			auto& info = m_enums[typeId];
			info.typeId = typeId;
			return info;
		}
		ClassInfo& registerClassInfo(TypeId typeId) {
			auto& info = m_classes[typeId];
			info.typeId = typeId;
			return info;
		}

	private:
		HashMap<TypeId, EnumInfo> m_enums;
		HashMap<TypeId, ClassInfo> m_classes;
		//HashMap<TypeId, PrimitiveInfo> m_primitives;


	};
}
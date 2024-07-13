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

		void visit(const std::function<void(const TypeInfo&)> func) {
			for (auto& [type, info] : m_infos) func(info);
		}

		const TypeInfo* find(Type type)const {
			if (auto itr = m_infos.find(type); itr != m_infos.end()) {
				return &itr->second;
			}
			return nullptr;
		}
		const TypeInfo* find(StringView type)const {
			return find(Type(type));
		}


		TypeInfo& registerInfo(Type type) {
			auto& info = m_infos[type];
			info = {};
			info.type = type;
			return info;
		}

	private:
		HashMap<Type, TypeInfo> m_infos;
	};
}
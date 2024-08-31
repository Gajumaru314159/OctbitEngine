//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

namespace ob::core {


	TypeInfoManager::TypeInfoManager() {
		// TODO Primitive型

		// 
		auto f = type_info_builder::GetReflectionFunction();
		while (f != nullptr) {
			f->func();
			f = f->next;
		}

		// 継承解決
		for (auto& [type, info] : m_infos) {
			for (auto& baseClass : info.bases) {
				if (auto baseInfo = find(baseClass)) {
					// baseInfo.inherits.emplace(type);
				}
			}
		}

	}

	void TypeInfoManager::visit(const std::function<void(const TypeInfo&)> func) const {
		for (auto& [type, info] : m_infos) func(info);
	}

	const TypeInfo* TypeInfoManager::find(Type type)const {
		if (auto itr = m_infos.find(type); itr != m_infos.end()) {
			return &itr->second;
		}
		return nullptr;
	}
	const TypeInfo* TypeInfoManager::find(StringView type)const {
		return find(Type(type));
	}
	const TypeInfo* TypeInfoManager::find(Type::hash_type type)const {
		if (auto itr = m_infos.find(Type(type)); itr != m_infos.end()) {
			return &itr->second;
		}
		return nullptr;
	}


	TypeInfo& TypeInfoManager::registerInfo(Type type) {
		auto& info = m_infos[type];
		info = {};
		info.type = type;
		return info;
	}

}
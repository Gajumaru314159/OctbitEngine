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
		for (auto& [type, info] : m_classes) {
			for (auto& baseClass : info.bases) {
				auto itr = m_classes.find(baseClass);
				if (itr != m_classes.end()) {
					itr->second.derivedes.emplace(type);
				}
			}
		}

	}

}
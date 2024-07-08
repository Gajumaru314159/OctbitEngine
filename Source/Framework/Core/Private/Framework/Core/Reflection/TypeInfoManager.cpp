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

}
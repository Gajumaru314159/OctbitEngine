//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Core.h>

namespace ob::core {

	bool TypeInfo::isSuperClassOf(const Type& type)const {

		if(bases.count(type))return true;

		if (auto manager = TypeInfoManager::Get()) {
			for (auto& base : bases) {
				if (auto baseInfo = manager->find(base)) {
					if (baseInfo->isSuperClassOf(type))return true;
				}
			}
		}

		return false;
	}

}
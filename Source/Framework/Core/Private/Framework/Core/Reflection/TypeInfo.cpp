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

	bool TypeInfo::isBaseOf(const Type& super)const {

		if (auto info = TypeInfoManager::Find(super)) {
			return info->isSuperClassOf(type);
		}

		return false;
	}

	bool TypeInfo::isSuperClassOf(const Type& base)const {

		if(bases.count(base))return true;

		for (auto& base2 : bases) {
			if (auto info = TypeInfoManager::Find(base2)) {
				if (info->isSuperClassOf(base))return true;
			}
		}

		return false;
	}

}
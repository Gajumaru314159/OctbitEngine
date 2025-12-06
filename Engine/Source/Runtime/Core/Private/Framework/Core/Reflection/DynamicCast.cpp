#include <Framework/Core/Reflection/DynamicCast.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>

namespace ob::core {

	bool IsCastable(Type from, Type to) {
        if (from == to)return true;
        if (auto info = TypeInfoManager::Find(from)) {
            return info->isSuperClassOf(to);
        }
		return false;
	}


}
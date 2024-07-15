//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief		内部型を取得する
    //@―---------------------------------------------------------------------------
    bool Any::is(const Type& t) const {
        if (type() == t)return true;
        if (auto manager = TypeInfoManager::Get()) {
            if (auto info = manager->find(type())) {
                if (info->isSuperClassOf(t)) {
                    return true;
                }
            }
        }
        return false;
    }

}
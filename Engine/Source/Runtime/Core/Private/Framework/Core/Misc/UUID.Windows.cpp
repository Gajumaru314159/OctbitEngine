//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/UUID.h>

#ifdef OS_WINDOWS
#include <objbase.h>

namespace ob::core {

    //! @brief 新しいUUIDを生成
    UUID UUID::Generate() {
        UUID uuid;
#ifdef OS_WINDOWS
        HRESULT r = CoCreateGuid(reinterpret_cast<GUID*>(&uuid));
        OB_ASSERT(r == S_OK, "Failed to create UUID.");
#else
#pragma error("UUID::Generate() is not supported in this platform.")
#endif
        return uuid;
    }

}

#endif
//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/UUID.h>

#ifdef OS_LINUX
// sudo apt-get install uuid-dev
#include <uuid/uuid.h>

namespace ob::core {

    //! @brief 新しいUUIDを生成
    UUID UUID::Generate() {

        static_assert(sizeof(UUID)==sizeof(uuid_t));

        UUID result;

        uuid_t uuid; // UUIDを格納する変数を宣言
        uuid_generate(uuid); // UUIDを生成

        std::memcpy(&result, &uuid, sizeof(uuid_t));

        return result;
    }

}

#endif
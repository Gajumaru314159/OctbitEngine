//***********************************************************
//! @file
//! @brief Blob
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Blob.h>
#include <Framework/Core/File/Stream.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief ストリームから生成
    //@―---------------------------------------------------------------------------
    Blob::Blob(Stream& stream) {

        if (stream.canRead()) {
            resize(stream.size());
            stream.read(data(), size());
        }

    }

} 
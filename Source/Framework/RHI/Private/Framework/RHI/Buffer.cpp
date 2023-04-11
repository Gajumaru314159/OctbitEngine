//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //! @param desc バッファ定義
    //@―---------------------------------------------------------------------------
    Ref<Buffer> Buffer::Create(const BufferDesc& desc) {
        if (auto rhi= RHI::Get()) {
            return rhi->createBuffer(desc);
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //@―---------------------------------------------------------------------------
    Ref<Buffer> Buffer::Create(const BufferDesc& desc, BlobView blob) {
        auto buffer = Create(desc);
        if (buffer) {
            buffer->updateDirect(blob.size(), blob.data());
        }
        return buffer;
    }

}// namespace ob
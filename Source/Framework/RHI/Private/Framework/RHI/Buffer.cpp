//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //! @brief  空のバッファを作成
    Ref<Buffer> Buffer::Create(const BufferDesc& desc) {
        if (auto rhi= Device::Get()) {
            return rhi->createBuffer(desc);
        }
        return nullptr;
    }


    //! @brief  初期データを指定してバッファを作成
    Ref<Buffer> Buffer::Create(const BufferDesc& desc, BlobView blob) {
        auto buffer = Create(desc);
        if (buffer) {
            buffer->updateDirect(blob.size(), blob.data());
        }
        return buffer;
    }


    //! @brief  ベースのバッファを指定して異なるビューを持つバッファを作成
    Ref<Buffer> Buffer::Create(const BufferViewDesc& desc) {
        if (auto rhi = Device::Get()) {
            return rhi->createBuffer(desc);
        }
        return nullptr;
    }

}
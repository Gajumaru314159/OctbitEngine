//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Buffer.h>
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Interface/IBuffer.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Buffer);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のBufferへアクセス")


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Buffer::Buffer() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Buffer::Buffer(const BufferDesc& desc, const Blob& data) {
        m_pImpl = Device::Get()->createBuffer(desc);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "スワップチェインの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const BufferDesc& Buffer::getDesc()const {
        CHECK_IMPL();
        return m_pImpl->getDesc();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バッファサイズを取得
    //@―---------------------------------------------------------------------------
    size_t Buffer::size()const {
        return getDesc().bufferSize;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  GPUリソースにCPU仮想アドレスを割り当てる
    //@―---------------------------------------------------------------------------
    void Buffer::beginUpdate(UpdateResourceParameter& param) {
        CHECK_IMPL();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  マップを解除する
    //@―---------------------------------------------------------------------------
    void Buffer::endUpdate(UpdateResourceParameter& param) {
        CHECK_IMPL();
    }

}// namespace ob
//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/IBuffer.h>

namespace ob::rhi {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Buffer);

#define CHECK_IMPL() OB_ASSERT(m_pImpl,"未初期化のBufferへアクセス")


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Buffer::Buffer() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc バッファ定義
    //@―---------------------------------------------------------------------------
    Buffer::Buffer(const BufferDesc& desc) {
        m_pImpl = Device::Get()->createBuffer(desc);
        OB_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "スワップチェインの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Buffer::Buffer(const BufferDesc& desc, BlobView blob) {
        m_pImpl = Device::Get()->createBuffer(desc);
        OB_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "スワップチェインの生成に失敗");
            release();
        }
        update(blob.size(), blob.data());
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
    //! @brief      バッファを更新
    //! 
    //! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
    //@―---------------------------------------------------------------------------
    void Buffer::update(size_t size, const void* pData, size_t offset) {
        // TODO
        updateDirect(size,pData,offset);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      バッファを更新
    //! 
    //! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
    //@―---------------------------------------------------------------------------
    void Buffer::updateDirect(size_t size, const void* pData, size_t offset) {
        CHECK_IMPL();
        m_pImpl->updateDirect(size, pData, offset);
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
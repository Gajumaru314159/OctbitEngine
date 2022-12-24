//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/IDescriptorTable.h>

namespace ob::rhi {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(DescriptorTable);

#define CHECK_IMPL() OB_ASSERT(m_pImpl,"未初期化のDescriptorTableへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //!
    //! @param type         デスクリプタに設定するリソースの種類
    //! @param elementNum   要素数
    //! @param name         デバッグ名
    //@―---------------------------------------------------------------------------
    DescriptorTable::DescriptorTable(DescriptorHeapType type, s32 elementNum) {
        m_pImpl = Device::Get()->createDescriptorTable(type,elementNum);
        OB_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "DescriptorTableの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  リソースを設定
    //@―---------------------------------------------------------------------------
    //! @{
    bool DescriptorTable::setResource(s32 index, class Buffer& resource) {
        CHECK_IMPL();
        return m_pImpl->setResource(index, resource);
    }
    bool DescriptorTable::setResource(s32 index, class Texture& resource) {
        CHECK_IMPL();
        return m_pImpl->setResource(index, resource);
    }
    bool DescriptorTable::setResource(s32 index, class Sampler& resource) {
        CHECK_IMPL();
        //m_pImpl->setResource(index, resource);
        OB_NOTIMPLEMENTED();
        return false;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  リソースのバインドを解除
    //@―---------------------------------------------------------------------------
    void DescriptorTable::clear() {
        CHECK_IMPL();
        m_pImpl->clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  指定したインデックスのリソースのバインドを解除
    //@―---------------------------------------------------------------------------
    void DescriptorTable::clearAt(s32 index) {
        CHECK_IMPL();
        m_pImpl->clearAt(index);
    }

}// namespace ob
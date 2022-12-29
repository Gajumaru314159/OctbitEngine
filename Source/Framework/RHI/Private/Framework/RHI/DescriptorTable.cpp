//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //!
    //! @param type         デスクリプタに設定するリソースの種類
    //! @param elementNum   要素数
    //! @param name         デバッグ名
    //@―---------------------------------------------------------------------------
    Ref<DescriptorTable> DescriptorTable::Create(DescriptorHeapType type, s32 elementNum) {
        return Device::Get()->createDescriptorTable(type, elementNum);
    }

}// namespace ob
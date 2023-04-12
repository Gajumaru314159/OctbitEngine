//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //!
    //! @param type         デスクリプタに設定するリソースの種類
    //! @param elementNum   要素数
    //! @param name         デバッグ名
    //@―---------------------------------------------------------------------------
    Ref<DescriptorTable> DescriptorTable::Create(DescriptorHeapType type, s32 elementNum) {
        if (auto rhi= RHI::Get()) {
            return rhi->createDescriptorTable(type, elementNum);
        }
        return nullptr;
    }

}
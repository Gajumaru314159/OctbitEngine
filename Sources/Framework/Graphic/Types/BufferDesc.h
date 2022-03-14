//***********************************************************
//! @file
//! @brief		バッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/ResourceUsage.h>
#include <Framework/Graphic/Types/BindFlag.h>
#include <Framework/Graphic/Types/BufferFlag.h>
#include <Framework/Graphic/Types/BufferType.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  バッファ定義
    //@―---------------------------------------------------------------------------
    struct BufferDesc{
        BufferType      bufferType;     //!< バッファタイプ
        ResourceUsage   usage;          //!< リソース使用法
        u32             bufferSize;     //!< バッファサイズ
        u32             bufferStride;   //!< ストライブ幅
        BufferFags      bufferFlags;    //!< バッファフラグ
        BindFlags       bindFlags;      //!< バインドフラグ
    };

}// namespcae ob::graphic
﻿//***********************************************************
//! @file
//! @brief		デスクリプタ定義
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  ディスクリプタ・タイプ
    //@―---------------------------------------------------------------------------
    enum class DescriptorHeapType :u32 {
        CBV_SRV_UAV,    //!< 定数バッファ/シェーダリソース/アンオーダードアクセス
        Sampler,        //!< サンプラー
        RTV,            //!< レンダーターゲット
        DSV,            //!< デプスステンシル
    };

}// namespcae ob::graphic
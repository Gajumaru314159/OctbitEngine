//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  結果
    //@―---------------------------------------------------------------------------
    enum class Result{
        Succeed,                //!< 成功

        OutOfRange,             //!< 範囲外アクセス
        DeviceRemoved,          //!< デバイスの取り外し(応答停止)
        InvalidParameter,       //!< 不正なパラメータ
        NotSupported,           //!< 未サポートの機能
        OutOfSystemMemory,      //!< システム(RAM)のメモリ超過
        OutOfDeviceMemory,      //!< デバイス(GPU)のメモリ超過
        OutOfPoolMemory,        //!< メモリープール超過
        InvalidCall,            //!< 不正な呼び出し
        TooManyObjects,         //!< オブジェクトの最大数超過
        ResourceSizeExceeded,   //!< リソースサイズを超過
        UsageIncompatibe,       //!< 互換性のない使い方
    };


}// namespcae ob::rhi
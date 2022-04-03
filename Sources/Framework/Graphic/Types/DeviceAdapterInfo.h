﻿//***********************************************************
//! @file
//! @brief		デバイス・アダプタ・インフォ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      デバイス・アダプタ・インフォ
    //! 
    //! @details    GPU情報
    //@―---------------------------------------------------------------------------
    struct DeviceAdapterInfo {
        char    deviceName[256];        //!< デバイス名
        u32     vendorId;               //!< ベンダーID
        u32     deviceId;               //!< デバイスID
        u8      adapterLUID[8];         //!< OS終了時まで固有のID
        size_t  dedicatedVideoMemory;   //!< デバイス専用のメモリサイズ
        size_t  sharedSystemMemory;     //!< デバイスとホストが共有可能なメモリサイズ
        s32     nodeCount;              //!< アダプタが抽象化している物理的なGPUの数
    };

}// namespace pb::graphic
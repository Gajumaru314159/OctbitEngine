﻿//***********************************************************
//! @file
//! @brief		ヒープ・使用法
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob
{
    //@―---------------------------------------------------------------------------
    //! @brief ヒープ・使用法
    //@―---------------------------------------------------------------------------
    enum class HeapUsage
    {
        VirtualMemory = 0,      //!< 仮想メモリ
        CPUCached,              //!< CPU側キャッシュ

        GPUReadable,            //!< GPU読み込み
        GPUReadWritable,        //!< GPU読み書き

        Max
    };

}// namespcae ob
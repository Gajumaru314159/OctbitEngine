﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#ifdef OB_DEBUG

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class PIXModule {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        PIXModule();
        ~PIXModule();

        void release();

    private:

        HMODULE m_hModule;


    };

}

#endif// OB_DEBUG
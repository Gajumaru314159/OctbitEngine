﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief      コマンドキュー(D3D12)
        //! 
        //! @details    コマンドキューは複数のコマンドリストをGPUに送信するために使用されます。
        //!             コマンドリストは非スレッドセーフのため、スレッドごとに構築された
        //!             コマンドリストをまとめるために使用されます。
        //@―---------------------------------------------------------------------------
        class D3D12CommandQueue{
        public:

            //===============================================================
            // コンストラクタ / デストラクタ
            //===============================================================

            //@―---------------------------------------------------------------------------
            //! @brief  説明
            //@―---------------------------------------------------------------------------

        private:

            void executeCommandListsImpl();

        };

    }// namespace graphic
}// namespcae ob

// CommandAllocator
// CommandList
// CommandQueue
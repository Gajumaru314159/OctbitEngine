﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    enum class CommandListType {
        Direct,
        Compute,
    };

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class CommandList :public RefObject {
    public:

        //===============================================================
        // コマンド
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        void begin();
        void end();

    private:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        CommandList(CommandListType type);


    };

}// namespace pb::graphic
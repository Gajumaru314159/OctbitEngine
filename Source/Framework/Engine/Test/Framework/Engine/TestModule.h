﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>

namespace ob::engine::test {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class TestModule : public IModule {
    public:

        static Name Name() {
            static ob::core::Name name(TC("TestModule"));
            return name;
        }

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        TestModule(Engine& engine)
            : IModule(engine)
        {
        }

    private:

    };

}// namespcae ob
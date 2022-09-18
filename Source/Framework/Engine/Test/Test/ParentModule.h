//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/Engine/Engine.h>
#include <Test/TestModule.h>

namespace ob::engine::test {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class ParentModule : public IModule {
    public:

        static Name Name() {
            static ob::core::Name name(TC("ParentModule"));
            return name;
        }

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        ParentModule(Engine& engine)
            : IModule(engine)
        {
            engine.<TestModule>();
        }

    private:

    };

}// namespcae ob
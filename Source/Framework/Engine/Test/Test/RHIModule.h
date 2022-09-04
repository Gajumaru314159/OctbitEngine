//***********************************************************
//! @file
//! @brief		RHIベースモジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>

namespace ob::engine::test {

    class ModuleManager {
    public:
        class ModuleInfo {
        public:
            template<class T>
            ModuleInfo& setFallbackAs();
            template<class T>
            ModuleInfo& addRequire();
            template<class T>
            ModuleInfo& addOption();
        };
    public:

        template<class T>
        ModuleInfo& add();

    };

    class PlatformModule;
    class PIXModule;

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class RHIModule : public IModule {
    public:

        static void Register(ModuleManager& manager) {
            manager.add<RHIModule>();
        }

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        RHIModule(Engine& engine)
            : IModule(engine)
        {
        }

    private:

    };

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DirectXModule : public RHIModule {
    public:

        static void Register(ModuleManager& manager) {
            manager.add<DirectXModule>()
                .addRequire<PlatformModule>()
                .addOption<PIXModule>();
        }

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        DirectXModule(Engine& engine)
            : RHIModule(engine)
        {
        }

    private:

    };

}// namespcae ob
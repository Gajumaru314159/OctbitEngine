//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "IModule.h"


namespace ob::platform {
    class ModuleLoader;
}

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief      モジュール・マネージャ
    //! 
    //! @details    DLL(動的ライブラリ)の読み込みや取得をサポートする。@n
    //!             管理するDLLはCreateModule関数を通してIModuleを継承したモジュール
    //!             を返す必要がある。返されるモジュールは唯一のものであり、再度
    //!             CreateModule関数を呼び出した場合はアサートを発生させることが望ま
    //!             れる。
    //!             @par
    //!             ロードしたモジュールが明示的に解放されない場合は、ロードと逆の順
    //!             番で解放される。ただしモジュールを通して生成されたインスタンスが
    //!             事前に解放されている必要があるため、このマネージャは最初に初期化
    //!             され最後に解放されることが望ましい。(モジュールを使用しないマネ
    //!             ージャを除く)
    //!             @par
    //!             使用例@n
    //! ```
    //!             auto pSampleModule = ModuleManager::get().LoadModule<ISampleModule>("SampleModule.dll");
    //!             {
    //!                 auto pSampleInstance = unique_ptr<ISampleInstance>(pSampleModule->CreateInstance());
    //!                 pSampleInstance->Function();
    //!             }
    //!             ModuleManager::get().UnloadModule(pSampleModule);
    //! ```
    //@―---------------------------------------------------------------------------
    class ModuleManager :public Singleton<ModuleManager> {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  モジュールを読み込み
        //@―---------------------------------------------------------------------------
        template<typename TModule>
        TModule* LoadModule(StringView moduleName) {
            IModule* pModule = LoadModuleImpl(moduleName);
            return reinterpret_cast<TModule*>(pModule);
        }


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールを破棄
        //@―---------------------------------------------------------------------------
        void UnloadModule(StringView moduleName);


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールを取得
        //@―---------------------------------------------------------------------------
        IModule* GetModule(StringView moduleName)const;


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールが読み込まれているか
        //@―---------------------------------------------------------------------------
        bool HasLoadedModule(StringView moduleName)const;


    private:

        IModule* LoadModuleImpl(StringView moduleName);


    private:

        std::map<StringView, ModuleLoader*> m_moduleMap;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------

}// namespace ob::platform
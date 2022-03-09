//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "IModule.h"

namespace ob::platform {

    class ModuleLoader;

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
    //!             auto pSampleModule = ModuleManager::Get().LoadModule<ISampleModule>("SampleModule.dll");
    //!             {
    //!                 auto pSampleInstance = unique_ptr<ISampleInstance>(pSampleModule->CreateInstance());
    //!                 pSampleInstance->Function();
    //!             }
    //!             ModuleManager::Get().UnloadModule(pSampleModule);
    //! ```
    //@―---------------------------------------------------------------------------
    class ModuleManager :public singleton<ModuleManager> {
    public:


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールの読み込み
        //@―---------------------------------------------------------------------------
        template<typename TModule>
        TModule* LoadModule(const String& moduleName) {
            IModule* pModule = LoadModuleImpl(moduleName);
            return reinterpret_cast<TModule*>(pModule);
        }


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールの破棄
        //@―---------------------------------------------------------------------------
        void UnloadModule(const String& moduleName);


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールの取得
        //@―---------------------------------------------------------------------------
        IModule* GetModule(const String& moduleName);


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールが読み込まれているか
        //@―---------------------------------------------------------------------------
        bool HasLoadedModule(const String& moduleName)const;


    private:

        IModule* LoadModuleImpl(const String& moduleName);


    private:

        ob::map<String, std::unique_ptr<ModuleLoader>> m_moduleMap;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------

}// namespace ob::platform
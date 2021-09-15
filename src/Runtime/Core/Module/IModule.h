//***********************************************************
//! @file
//! @brief		モジュール・インターフェース
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //-----------------------------------------------------------------------------
    //! @brief      モジュール・インターフェース
    //! 
    //! @details    すべてのモジュールが実装すべきインターフェースクラスです。 
    //!             これは、モジュールがロードされた後に初期化するために使用されます。
    //!             モジュールがロードされた後の初期化と、モジュールがアンロードされる前のクリーンアップに使用されます。
    //-----------------------------------------------------------------------------
    class IModule {
    public:

        virtual ~IModule() = 0;                   //!< デストラクタ
        virtual void Startup() {};              //!< DLLが読み込まれ、モジュールオブジェクトが生成されたときに呼び出される。
        virtual void PreUnloadCallback() {};    //!< 
        virtual void PostLoadCallback() {};     //!< 
        virtual void ShutdownModule() {};       //!< 

        virtual bool SupportsDynamicReloading()const { return true; };
        virtual bool SupportsAutomaticShutdown()const { return true; };
        virtual bool IsGameModule()const { return false; };

    private:



    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------


}// namespcae ob
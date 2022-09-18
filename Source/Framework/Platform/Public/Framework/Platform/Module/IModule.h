﻿//***********************************************************
//! @file
//! @brief		モジュール・インターフェース
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Memory/Allocator/Allocator.h>
#include <Framework/Core/Memory/System/MemorySystem.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief      モジュール・インターフェース
    //! 
    //! @details    すべてのモジュールが実装すべきインターフェースクラスです。 
    //!             これは、モジュールがロードされた後に初期化するために使用されます。
    //!             モジュールがロードされた後の初期化と、モジュールがアンロードされる前のクリーンアップに使用されます。
    //!             1. startup
    //!             2. PostLoadCallback
    //!             3. メイン動作
    //!             4. PreUnloadCallback
    //!             5. shutdown
    //@―---------------------------------------------------------------------------
    class IModule {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~IModule() = default;                   //!< デストラクタ


        //@―---------------------------------------------------------------------------
        //! @brief マジックコードを取得
        //@―---------------------------------------------------------------------------
        virtual u32 magicCode()const=0;


        //@―---------------------------------------------------------------------------
        //! @brief 起動
        //@―---------------------------------------------------------------------------
        virtual void startup(ob::core::Allocator* pAllocator)=0;


        //@―---------------------------------------------------------------------------
        //! @brief 終了
        //@―---------------------------------------------------------------------------
        virtual void shutdown() {};

    };

}// namespace ob::platform
//***********************************************************
//! @file
//! @brief		モジュール
//! @author		Gajumaru
//***********************************************************
#include "Module.h"
#include "Implement/Windows/ModuleLoader.h"

namespace ob {
    namespace platform {

        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ
        //! 
        //! @details        動的ライブラリからIModuleを継承するインターフェイスを生成/取得する。
        //!                 動的ライブラリはプラットフォームごとに拡張子が異なるので拡張子は
        //!                 記述しません。
        //! @param fileName 動的ライブラリのファイル名
        //@―---------------------------------------------------------------------------
        Module::Module(const String& filePath) {
            m_loader = std::make_unique<ModuleLoader>(filePath);
        }

    }// namespace platform
}// namespace ob
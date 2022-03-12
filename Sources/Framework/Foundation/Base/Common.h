//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Platform.h"		    // プラットフォーム識別
#include "BuildConfig.h"		// コンフィグ読み込み
#include "Macro.h"				// マクロ
#include "PrimitiveType.h"		// プリミティブ型


//============================================
// 定義
//============================================

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief		クラスコピーの禁止
    //! 
    //! @details	このクラスをprivate継承すると外部からのコピーコンストラクタと代入演
    //!				算子の呼び出しが禁止される。
    //@―---------------------------------------------------------------------------
    class Noncopyable {
    protected:
        //! @cond
        Noncopyable() {}
        ~Noncopyable() {}
    private:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
        //! @endvond
    };

    //@―---------------------------------------------------------------------------
    //! @brief		クラスムーブの禁止
    //! 
    //! @details	このクラスをprivate継承すると外部からのムーブコンストラクタとムーブ
    //!				代入演算子の呼び出しが禁止される。
    //@―---------------------------------------------------------------------------
    class Nonmovable {
    protected:
        //! @cond
        Nonmovable() {}
        ~Nonmovable() {}
    private:
        Nonmovable(Nonmovable&&) = delete;
        Nonmovable& operator=(Nonmovable&&) = delete;
        //! @endcond
    };

}// namespace ob


//============================================
// DLLエクスポート/インポート
//============================================

#if defined(OB_BUILD_DLL)
#define OB_API //DLL_EXPORT
#elif defined(OB_USE_DLL)
#define OB_API //DLL_IMPORT
#else
#define OB_API /**/
#endif
//@―---------------------------------------------------------------------------
//! @def		OB_API
//! @brief		DLLエクスポート/インポートマクロ
//! @details	DLL出力 / DLL入力 / その他(LIB)
//@―---------------------------------------------------------------------------


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief		強制初期化フラグ
    //! 
    //! @details	コンストラクタの引数に指定することで、メンバの初期化を指定する。@n
    //!				初期化内容は各構造体のリファレンスを確認してください。
    //@―---------------------------------------------------------------------------
    enum class EForceInit {
        Force,
    };

}// namespace ob
//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include<cassert>
#include "Platform/PlatformDefine.h"
#include "PrimitiveType.h"
#include "../gsl/gsl_loader.h"


//@―---------------------------------------------------------------------------
//! @def    OB_DEBUG
//! @brief  デバッグフラグ
//@―---------------------------------------------------------------------------
#if defined(_DEBUG)||defined(DEBUG)
#define OB_DEBUG
#endif


//@―---------------------------------------------------------------------------
//! @def    OB_DEBUG_ONLY
//! @brief  デバッグのみで使用する文脈
//@―---------------------------------------------------------------------------
#ifdef OB_DEBUG    
#define OB_DEBUG_ONLY(context) (context)
#else
#define OB_DEBUG_ONLY(context) /*空白置き換え*/
#endif


//@―---------------------------------------------------------------------------
//! @brief  未使用変数の警告抑制
//@―---------------------------------------------------------------------------
#define OB_UNUSED(x)                    (void)(x)


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
		Noncopyable(const Noncopyable&);
		Noncopyable& operator=(const Noncopyable&);
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
		Nonmovable(Nonmovable&&);
		Nonmovable& operator=(Nonmovable&&);
		//! @endcond
	};

}

//@―---------------------------------------------------------------------------
//! @brief		デフォルトコピーコンストラクタの定義
//@―---------------------------------------------------------------------------
#define DEFAULT_COPY_CONSTRUCTOR(typeName)	\
	typeName(const typeName&)=default;		\
	typeName& operator=(const typeName&)=default


//@―---------------------------------------------------------------------------
//! @brief		デフォルトコピーコンストラクタの定義
//@―---------------------------------------------------------------------------
#define DEFAULT_MOVE_CONSTRUCTOR(typeName)	\
	typeName(typeName&&)=default;		\
	typeName& operator=(typeName&&)=default

//============================================
// DLLエクスポート/インポート
//============================================

//@―---------------------------------------------------------------------------
//! @def		OB_API
//! @brief		DLLエクスポート/インポートマクロ
//! @details	DLL出力 / DLL入力 / その他(LIB)
//@―---------------------------------------------------------------------------
#if   defined(_WIN32) && defined(OB_BUILD_DLL)
#define OB_API __declspec(dllexport)
#elif defined(_WIN32) && defined(OB_USE_DLL)
#define OB_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(OB_BUILD_DLL)
#define OB_API __attribute__((visibility("default")))
#else
#define OB_API
#endif



//============================================
// コンパイラ依存
//============================================

//@―---------------------------------------------------------------------------
//! @def	__FUNC_NAME__
//! @brief	関数名取得マクロ(詳細版)
//@―---------------------------------------------------------------------------
#if defined(__GNUC__)
#define __FUNC_NAME__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define __FUNC_NAME__ __FUNCSIG__
#else
#define __FUNC_NAME__ __func__
#endif


namespace ob {

	//@―---------------------------------------------------------------------------
	//! @brief		強制0初期化フラグ
	//! 
	//! @details	コンストラクタの引数に指定することで、メンバの初期化を指定する。@n
	//!				初期化内容は各構造体のリファレンスを確認してください。
	//@―---------------------------------------------------------------------------
	enum class EZeroInit{
		Force,
	};


}// namespace ob

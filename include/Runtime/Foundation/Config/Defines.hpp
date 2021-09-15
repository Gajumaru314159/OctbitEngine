//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Config/Platform.hpp>

#ifdef OB_OS_WINDOWS
#	include "Windows/DefinesImpl.hpp"
#else

#endif


//-----------------------------------------------------------
//! @def    OB_DEBUG_ONLY
//! @brief  デバッグのみで使用する文脈
//-----------------------------------------------------------
#ifdef OB_DEBUG    
#define OB_DEBUG_ONLY(context) (context)
#else
#define OB_DEBUG_ONLY(context) /*空白置き換え*/
#endif

//-----------------------------------------------------------
//! @brief  未使用変数の警告抑制
//-----------------------------------------------------------
#define OB_UNUSED(x)                    (void)(x)

//-----------------------------------------------------------
//! @brief  静的アサートの発生
//-----------------------------------------------------------
#define OB_STATIC_ASSERT(expr,str)      static_assert((expr),str)




//-----------------------------------------------------------
//! @def		OB_LIKELY
//! @brief		コンパイラ最適化オプション
//! @details    式が真であることが多い場合コンパイルが最適化される
//-----------------------------------------------------------

//-----------------------------------------------------------
//! @def		OB_UNLIKELY
//! @brief		コンパイラ最適化オプション
//! @details	式が偽であることが多い場合コンパイルが最適化される 
//-----------------------------------------------------------
#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#	define OB_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#	define OB_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#else
#	define OB_LIKELY(expr) (1 == !!(expr))
#	define OB_UNLIKELY(expr) (0 == !!(expr))
#endif



// ============================================
// メモリ
// ============================================

//! @brief free関数によるnullチェック付き解放
#define OB_SAFE_FREE( p ) { if ( p ) { free( ( p ) ); ( p ) = NULL; } }
//! @brief deleteによるnullチェック付き解放
#define OB_SAFE_DELETE( p ) { if ( p ) { delete ( p ); ( p ) = NULL; } }
//! @brief delete[]によるnullチェック付き解放
#define OB_SAFE_DELETE_ARRAY( p ) { if ( p ) { delete[] ( p ); ( p ) = NULL; } }


//! @brief nullチェック付きAddRef
#define OB_COM_SAFE_ADDREF(p)	{ if (p) { (p)->AddRef(); } }
//! @brief nullチェック付きRelease
#define OB_COM_SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p)= nullptr; } }



//============================================
// 定義
//============================================


//-----------------------------------------------------------
//! @brief		クラスコピーの禁止
//! @details	コピーコンストラクタと代入演算子の削除。
//-----------------------------------------------------------
#define OB_DISALLOW_COPY(typeName) \
	typeName(const typeName&) = delete; \
	void operator=(const typeName&) = delete



//-----------------------------------------------------------
//! @def		OB_CONSTEXPR
//! @brief		constexper
//! @details	環境によってはconstexperがないためマクロで置換する。
//-----------------------------------------------------------
#if defined(__clang__)
#define OB_CONSTEXPR constexpr
#elif defined(__GNUC__) || defined(__GNUG__)
#define OB_CONSTEXPR
#elif !defined(_MSC_VER) || _MSC_VER >= 1900
#define OB_CONSTEXPR constexpr
#else
#define OB_CONSTEXPR
#endif


//-----------------------------------------------------------
//! @def		OB_NOEXCEPT
//! @brief		noexcept
//! @details	環境によってはnoexceptがないためマクロで置換する。
//-----------------------------------------------------------
#if defined(_MSC_VER) && _MSC_VER < 1900
#define OB_NOEXCEPT
#define OB_DEPRECATED
#define OB_NODISCARD
#else
#define OB_NOEXCEPT		noexcept
// TODO アトリビュートのバージョン確認
#define OB_DEPRECATED	[[deprecated]]
#define OB_NODISCARD [[nodiscard]]
#endif


//-----------------------------------------------------------
//! @def		OB_DEPRECATED
//! @brief		[[deprecated]]
//! @details	非推奨属性。
//-----------------------------------------------------------

//-----------------------------------------------------------
//! @def		OB_NODISCARD
//! @brief		[[nodiscard]]
//! @details	戻り値の破棄を禁止する。
//-----------------------------------------------------------
#if defined(_MSC_VER) && 1911 <= _MSC_VER
#define OB_DEPRECATED	[[deprecated]]
#define OB_NODISCARD	[[nodiscard]]
#else
#define OB_DEPRECATED
#define OB_NODISCARD
#endif
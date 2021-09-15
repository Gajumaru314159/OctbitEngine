//***********************************************************
//! @file
//! @brief		ハードウェア共通定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/BaseType.hpp>

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
//! @brief  静的配列の配列サイズを取得
//-----------------------------------------------------------
#define OB_ARRAY_SIZE(arr)              (sizeof(arr)/sizeof(arr[0]))


namespace ob
{

    //-----------------------------------------------------------
    //! @brief  大きいほうの値を返す
    //-----------------------------------------------------------
    template<typename T>inline T OB_MAX(const T& a, const T& b) { return (a > b) ? b : a; }

    //-----------------------------------------------------------
    //! @brief  小さいほうの値を返す
    //-----------------------------------------------------------
    template<typename T>inline T OB_MIN(const T& a, const T& b) { return (a < b) ? b : a; }

    //-----------------------------------------------------------
    //! @brief  値valをmin以上max以下に制限する
    //-----------------------------------------------------------
    template<typename T>inline T OB_CLAMP(const T& val, const T& min, const T& max) { return OB_MIN(OB_MAX(val, min), max); }

    //-----------------------------------------------------------
    //! @brief  値valをラップラウンドする
    //-----------------------------------------------------------
    template<typename T>inline T OB_WRAP(T val, const T& min, const T& max)
    {
        const T range = max - min;
        while (max <= val)val -= range;
        while (val < min)val += range;
        return val;
    }

    //-----------------------------------------------------------
    //! @brief  スワップ
    //-----------------------------------------------------------
    template<typename T>inline void OB_SWAP(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = a;
    }

}// namespcae ob


//============================================
// コンパイラ
//============================================

// デバッグモード
#if defined(DEBUG) || defined(_DEBUG)
//! @brief 定義されている場合デバッグモードを表す
#define OB_DEBUG
#endif


#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#	define OB_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#	define OB_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#else
#	define OB_LIKELY(expr) (1 == !!(expr))
#	define OB_UNLIKELY(expr) (0 == !!(expr))
#endif

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



#if defined(__GNUC__)
//! @brief GNUC(gcc)環境
#define OB_GNUC
#elif defined(_MSC_VER)
//! @brief MSVC環境
#define OB_MSVC
#else
#error "invalid compiler"
#endif




// ============================================
// プラットフォーム
// ============================================
#if defined(__EMSCRIPTEN__)
//! @brief Emscripten環境
#define OB_EMSCRIPTEN
//! @brief Emscripten環境
#define OB_OS_FAMILY_UNIX
#elif defined(__ANDROID__)
//! @brief Android環境
#define OB_OS_ANDROID
#define OB_OS_FAMILY_UNIX
#elif (defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(Q_OS_WIN))
//! @brief Windows環境
#define OB_OS_WIN
#define OB_OS_WIN32
#define OB_OS_DESKTOP
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
//! @brief iOS環境
#define OB_OS_DARWIN
#define OB_OS_FAMILY_UNIX
#else
//! @brief Linux環境
#define OB_OS_LINUX
#define OB_X11
#define OB_OS_FAMILY_UNIX
#define OB_OS_DESKTOP
#endif

#if defined(OB_OS_DARWIN)
#	include <TargetConditionals.h>
#	if (defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#		define OB_OS_IOS
#	elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
#		define OB_OS_MAC
#       define OB_OS_DESKTOP
#	endif
#endif



// ============================================
// DLLエクスポート/インポート
// ============================================

#if   defined(_WIN32) && defined(OB_BUILD_DLL)
#define OB_API __declspec(dllexport)
#elif defined(_WIN32) && defined(OB_USE_DLL)
#define OB_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(OB_BUILD_DLL)
#define OB_API __attribute__((visibility("default")))
#else
#define OB_API
#endif

//! @def OB_API
//! @brief DLLエクスポート/インポートマクロ



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
#else
#define OB_NOEXCEPT noexcept
#endif
//***********************************************************
//! @file
//! @brief		デファイン(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond

#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#define NOMINMAX                // min / max マクロを使わない

//-----------------------------------------------------------------------------
// インクルード
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <WinSock2.h>

#ifdef _DEBUG
#include <crtdbg.h> 
#endif

//-----------------------------------------------------------------------------
// ワーニング抑制
//-----------------------------------------------------------------------------
#pragma warning(disable:4018)   // signedとunsignedの数値を比較
#pragma warning(disable:4389)   // signedとunsignedの数値を等価比較
#pragma warning(disable:4324)   // 構造体のパディング自動追加
#pragma warning(disable:4127)   // 条件式が定数
#pragma warning(disable:4512)   // クラスの代入演算子が生成できない
#pragma warning(disable:4505)   // 参照されていないローカル関数の削除


//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLATFORM_NAME            "Windows"
#define ENTRY_POINT()            int WINAPI(HINSTANCE,HINSTANCE,LPSTR,int)   
#define ALIGN_DECL(type,alignment)  _declspec(align(alignment)) type
#define ALIGN_VAR(var,alignment)    _declspec(align(alignment)) var    
#define INLINE                   __forceinline
#define RESTRICT                 __restrict
#define DIRECTORY_CHAR           '\\'
#define DIRECTORY_STR            "\\"



//-----------------------------------------------------------------------------
//! @def		LIKELY
//! @brief		コンパイラ最適化オプション
//! @details    式が真であることが多い場合コンパイルが最適化される
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! @def		UNLIKELY
//! @brief		コンパイラ最適化オプション
//! @details	式が偽であることが多い場合コンパイルが最適化される 
//-----------------------------------------------------------------------------
#define LIKELY(expr)             (expr)
#define UNLIKELY(expr)           (expr)

#define OUTPUT_DEBUG_LOG(msg)       ::OutputDebugString(msg);\
                                    ::OutputDebugString("\n");
#define CALL_BREAK_POINT            if(::IsDebuggerPresent())_CrtDbgBreak
//! @endcond
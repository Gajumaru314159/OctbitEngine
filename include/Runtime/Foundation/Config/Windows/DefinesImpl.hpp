//***********************************************************
//! @file
//! @brief		デファイン(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once

#define WIN32_LEAN_AND_MEAN         // ヘッダーからあまり使われない関数を省く

//-----------------------------------------------------------
// インクルード
//-----------------------------------------------------------
#include <Windows.h>
#include <WinSock2.h>

#ifdef _DEBUG
#include <crtdbg.h> 
#endif

//-----------------------------------------------------------
// ワーニング抑制
//-----------------------------------------------------------
#pragma warning(disable:4018)   // signedとunsignedの数値を比較
#pragma warning(disable:4389)   // signedとunsignedの数値を等価比較
#pragma warning(disable:4324)   // 構造体のパディング自動追加
#pragma warning(disable:4127)   // 条件式が定数
#pragma warning(disable:4512)   // クラスの代入演算子が生成できない
#pragma warning(disable:4505)   // 参照されていないローカル関数の削除


//-----------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------
#define OB_PLATFORM_NAME            "Windows"
#define OB_ENTRY_POINT()            int WINAPI(HINSTANCE,HINSTANCE,LPSTR,int)   
#define ALIGN_DECL(type,alignment)  _declspec(align(alignment)) type
#define ALIGN_VAR(var,alignment)    _declspec(align(alignment)) var    
#define OB_INLINE                   __forceinline
#define OB_RESTRICT                 __restrict
#define OB_DIRECTORY_CHAR           '\\'
#define OB_DIRECTORY_STR            "\\"

#define OB_LIKELY(expr)             (expr)
#define OB_UNLIKELY(expr)           (expr)

#define OUTPUT_DEBUG_LOG(msg)       :OutputDebugString(msg);\
                                    :OutputDebugString("\n");
#define CALL_BREAK_POINT            if(::IsDebuggerPresent())_CrtDbgBreak
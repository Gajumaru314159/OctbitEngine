//***********************************************************
//! @file
//! @brief		デファイン(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond

#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#define NOMINMAX                // min / max マクロを使わない

//@―---------------------------------------------------------------------------
// インクルード
//@―---------------------------------------------------------------------------
#include <Windows.h>
#include <WinSock2.h>

#ifdef _DEBUG
#include <crtdbg.h> 
#endif

//@―---------------------------------------------------------------------------
// ワーニング抑制
//@―---------------------------------------------------------------------------
#pragma warning(disable:4018)   // signedとunsignedの数値を比較
#pragma warning(disable:4389)   // signedとunsignedの数値を等価比較
#pragma warning(disable:4324)   // 構造体のパディング自動追加
#pragma warning(disable:4127)   // 条件式が定数
#pragma warning(disable:4512)   // クラスの代入演算子が生成できない
#pragma warning(disable:4505)   // 参照されていないローカル関数の削除


//@―---------------------------------------------------------------------------
// マクロ定義
//@―---------------------------------------------------------------------------
#define PLATFORM_NAME_IMPL              "Windows"                                           // プラットフォーム名
#define ENTRY_POINT_IMPL()              int WINAPI(HINSTANCE,HINSTANCE,LPSTR,int)           // エントリーポイント
#define ALIGN_DECL_IMPL(type,alignment) _declspec(align(alignment)) type                    // アライン付き宣言
#define ALIGN_VAR_IMPL(var,alignment)   _declspec(align(alignment)) var                     // アライン付き変数宣言
#define FINLINE_IMPL                    __forceinline                                       // 強制inline修飾子
#define RESTRICT_IMPL                   __restrict                                          // restrict型修飾子
#define DIRECTORY_CHAR_IMPL             '\\'                                                // ディレクトリ文字
#define DIRECTORY_STR_IMPL              "\\"                                                // ディレクトリ文字列
#define LIKELY_IMPL(expr)               (expr)                                              // コンパイラ最適化オプション
#define UNLIKELY_IMPL(expr)             (expr)                                              // コンパイラ最適顔プsチョン
#define OUTPUT_DEBUG_LOG_IMPL(msg)      ::OutputDebugString(msg);\
                                        ::OutputDebugString("\n");                          // システムログ出力 
#define CALL_BREAK_POINT_IMPL           if(::IsDebuggerPresent())_CrtDbgBreak               // 特定のコード行にブレークポイントを設定する
//! @endcond
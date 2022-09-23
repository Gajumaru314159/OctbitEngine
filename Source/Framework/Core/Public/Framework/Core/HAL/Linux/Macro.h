//***********************************************************
//! @file
//! @brief		マクロ定義(Linux)
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond
#include <Framework/Core/HAL/Platform.h>
#if defined(OS_LINUX)

//@―---------------------------------------------------------------------------
// インクルード
//@―---------------------------------------------------------------------------


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
#define ENTRY_POINT_IMPL()              int main()                                          // エントリーポイント
#define ALIGN_DECL_IMPL(type,alignment) __attribute__ ((aligned(alignment))) type           // アライン付き宣言
#define ALIGN_VAR_IMPL(var,alignment)   __attribute__ ((aligned(alignment))) var            // アライン付き変数宣言
#define FINLINE_IMPL                    __attribute__((always_inline))                      // 強制inline修飾子
#define RESTRICT_IMPL                   __restrict                                          // restrict型修飾子
#define DIRECTORY_CHAR_IMPL             '/'                                                 // ディレクトリ文字
#define DIRECTORY_STR_IMPL              "/"                                                 // ディレクトリ文字列
#define LIKELY_IMPL(expr)               __builtin_expect(!!(expr), 1)                       // コンパイラ最適化オプション
#define UNLIKELY_IMPL(expr)             __builtin_expect(!!(expr), 0)                       // コンパイラ最適化オプション
#define OUTPUT_DEBUG_LOG_IMPL(msg)      /**/                                                // システムログ出力 
#define CALL_BREAK_POINT_IMPL           /**/                                                // 特定のコード行にブレークポイントを設定する
#define DLL_EXPORT_IMPL                 /**/                                                // DLL出力
#define DLL_IMPORT_IMPL                 /**/                                                // DLL入力
#define _internal_FUNC_NAME             __PRETTY_FUNCTION__                                 // 関数名

#endif
//! @endcond
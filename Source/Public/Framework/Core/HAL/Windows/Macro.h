//***********************************************************
//! @file
//! @brief		マクロ定義(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond
#include <Framework/Core/HAL/Platform.h>
#if defined(OS_WINDOWS)

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
#define ALIGN_DECL_IMPL(type,alignment) _declspec(align(alignment)) type                    // アライン付き宣言
#define ALIGN_VAR_IMPL(var,alignment)   _declspec(align(alignment)) var                     // アライン付き変数宣言
#define FINLINE_IMPL                    __forceinline                                       // 強制inline修飾子
#define RESTRICT_IMPL                   __restrict                                          // restrict型修飾子
#define DIRECTORY_CHAR_IMPL             '\\'                                                // ディレクトリ文字
#define DIRECTORY_STR_IMPL              "\\"                                                // ディレクトリ文字列
#define LIKELY_IMPL(expr)               (expr)                                              // コンパイラ最適化オプション
#define UNLIKELY_IMPL(expr)             (expr)                                              // コンパイラ最適化オプション
#define DLL_EXPORT_IMPL                 __declspec(dllexport)                               // DLL出力
#define DLL_IMPORT_IMPL                 __declspec(dllimport)                               // DLL入力
#define __FUNC_NAME__IMPL               __FUNCSIG__                                         // 関数名

#endif
//! @endcond
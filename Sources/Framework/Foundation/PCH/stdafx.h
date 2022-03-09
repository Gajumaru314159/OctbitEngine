//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#pragma once


//============================================
// System Base
//============================================
#include <Framework/Foundation/Base/Common.h>


//============================================
// Standard Template Library
//============================================
#include <cassert>          // assert
#include <stdint.h>         // 整数型
#include <stddef.h>         // offsetof ptrdiff_t size_t wchar_t
#include <Framework/Foundation/Template/STL/include.h>


//============================================
// Guidelines Support Library
// https://github.com/microsoft/GSL
//============================================
#pragma warning(push)
#pragma warning(disable:4003)  // min / max の警告抑制

#include <gsl/gsl_algorithm> // copy
#include <gsl/gsl_assert>    // Ensures/Expects
#include <gsl/gsl_byte>      // byte
#include <gsl/gsl_util>      // finally()/narrow()/narrow_cast()...
#include <gsl/multi_span>    // multi_span, strided_span...
#include <gsl/pointers>      // owner, not_null
#include <gsl/span>          // span
#include <gsl/string_span>   // zstring, string_span, zstring_builder...

#pragma warning(pop)


//============================================
// Template
//============================================
#include <Framework/Foundation/Template/Utility.h>
#include <Framework/Foundation/Template/Memory.h>
#include <Framework/Foundation/Template/Singleton.h>
//***********************************************************
//! @file
//! @brief		Guidelines Support Library(Microsoft)
//! @author		Gajumaru
//***********************************************************
#pragma once

#pragma warning(push)
#pragma warning(disable:4003)  // min / max ÇÃåxçêó}êß

#include <gsl/gsl_algorithm> // copy
#include <gsl/gsl_assert>    // Ensures/Expects
#include <gsl/gsl_byte>      // byte
#include <gsl/gsl_util>      // finally()/narrow()/narrow_cast()...
#include <gsl/multi_span>    // multi_span, strided_span...
#include <gsl/pointers>      // owner, not_null
#include <gsl/span>          // span
#include <gsl/string_span>   // zstring, string_span, zstring_builder...

#pragma warning(pop)
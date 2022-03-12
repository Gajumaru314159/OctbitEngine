﻿//======================================================================
//-----------------------------------------------------------------------
/**
 * @file        iutest_switch_pred.hpp
 * @brief       述語フォーマット 切り替え定義 ファイル
 *
 * @author      t.shirayanagi
 * @par         copyright
 * Copyright (C) 2013-2016, Takazumi Shirayanagi\n
 * This software is released under the new BSD License,
 * see LICENSE
*/
//-----------------------------------------------------------------------
//======================================================================
#ifndef INCG_IRIS_IUTEST_SWITCH_PRED_HPP_B00F77FE_0772_48EC_B182_DAB0AA3EA2DC_
#define INCG_IRIS_IUTEST_SWITCH_PRED_HPP_B00F77FE_0772_48EC_B182_DAB0AA3EA2DC_

#if !defined(IUTEST_USE_GTEST)

//======================================================================
// define

#else // !defined(IUTEST_USE_GTEST)

//======================================================================
// undef
#if defined(INCG_IRIS_IUTEST_HPP_)

#undef IUTEST_PRED_FORMAT1_
#undef IUTEST_PRED_FORMAT2_
#undef IUTEST_PRED_FORMAT3_
#undef IUTEST_PRED_FORMAT4_
#undef IUTEST_PRED_FORMAT5_

#ifdef IUTEST_PRED_
#  undef IUTEST_PRED_
#endif
#ifdef IUTEST_PRED_FORMAT_
#  undef IUTEST_PRED_FORMAT_
#endif

#endif

//======================================================================
// define
#define IUTEST_PRED_FORMAT1_    GTEST_PRED_FORMAT1_
#define IUTEST_PRED_FORMAT2_    GTEST_PRED_FORMAT2_
#define IUTEST_PRED_FORMAT3_    GTEST_PRED_FORMAT3_
#define IUTEST_PRED_FORMAT4_    GTEST_PRED_FORMAT4_
#define IUTEST_PRED_FORMAT5_    GTEST_PRED_FORMAT5_

#ifndef IUTEST_THROW_PRED_FORMAT2_
#  define IUTEST_THROW_PRED_FORMAT2_(pred_formatter, statement, expected_exception, expected_exception_value, on_failure)   \
    IUTEST_TEST_THROW_VALUE_(statement, expected_exception, expected_exception_value, on_failure, pred_formatter)
#endif

#endif // !defined(IUTEST_USE_GTEST)

#endif // INCG_IRIS_IUTEST_SWITCH_PRED_HPP_B00F77FE_0772_48EC_B182_DAB0AA3EA2DC_

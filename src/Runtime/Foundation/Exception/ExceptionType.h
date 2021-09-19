﻿//***********************************************************
//! @file
//! @brief		例外タイプ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob
{

	//@―---------------------------------------------------------------------------
	//! @brief 例外レベル
	//@―---------------------------------------------------------------------------
	enum class ExceptionLevel {
		Fatal,		//!< プログラムの継続が難しい致命的な例外。プログラムは直ちに終了しなければならない。
		Error,		//!< プログラムが不正な状態になる可能性がある例外。プログラムの継続が危険であり、早急に終了することが望まれる。
		Warning,	//!< プログラムの実行中に発生した軽微な例外。プログラムの継続が可能である。
	};

}// namespcae ob
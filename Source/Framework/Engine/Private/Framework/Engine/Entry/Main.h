﻿//***********************************************************
//! @file
//! @brief		エントリ
//! @author		Gajumaru
//***********************************************************
#pragma once

//@―---------------------------------------------------------------------------
//! @brief		エンジン起動設定
//! 
//! @details	アプリケーション起動直後に呼び出されます。
//@―---------------------------------------------------------------------------
void OctbitInit(ob::core::ServiceInjector& engineInjector);

//@―---------------------------------------------------------------------------
//! @brief		エンジンエントリ関数
//! 
//! @details	プラットフォームの初期化後に呼び出されます。
//@―---------------------------------------------------------------------------
int OctbitMain();
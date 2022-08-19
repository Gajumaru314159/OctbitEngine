﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Module/IModule.h>

//============================================
// DLL Import / Export
//============================================
#if defined(OB_GRAPHIC_DIRECTX12_BUILD)
#define OB_GRAPHIC_DIRECTX12_API DLL_EXPORT
#else
#define OB_GRAPHIC_DIRECTX12_API DLL_IMPORT
#endif


extern"C" {

    ob::platform::IModule OB_GRAPHIC_DIRECTX12_API *GetModule();

}
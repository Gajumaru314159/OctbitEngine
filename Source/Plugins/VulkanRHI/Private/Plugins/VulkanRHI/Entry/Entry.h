//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

//============================================
// DLL Import / Export
//============================================
#if defined(OB_GRAPHIC_VULKAN_BUILD)
#define OB_GRAPHIC_VULKAN_API DLL_EXPORT
#else
#define OB_GRAPHIC_VULKAN_API DLL_IMPORT
#endif

//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#pragma once


//============================================
// System Base
//============================================
#include <Framework/Foundation.h>
#include <Framework/Core.h>


//============================================
// DirectX
//============================================

#pragma warning(push)
#pragma warning(disable:4100)
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXTex/d3dx12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#pragma warning(pop)


//============================================
// DLL Import / Export
//============================================
#if defined(OB_GRAPHIC_DIRECTX12_BUILD)
#define OB_GRAPHIC_DIRECTX12_API DLL_EXPORT
#else
#define OB_GRAPHIC_DIRECTX12_API DLL_IMPORT
#endif

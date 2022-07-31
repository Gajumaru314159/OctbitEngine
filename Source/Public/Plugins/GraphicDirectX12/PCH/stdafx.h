//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#pragma once

//============================================
// System Base
//============================================
#include <Framework/Core.h>


//============================================
// DirectX
//============================================

#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#pragma warning(push)
#pragma warning(disable:4100)
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#pragma warning(pop)

//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#pragma once

//============================================
// System Base
//============================================
#include <Framework/Core/Core.h>


//============================================
// Vulkan
//============================================
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan_win32.h>
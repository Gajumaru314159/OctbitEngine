cmake_minimum_required (VERSION 3.15)
project ("OctbitEngine")

#------------------------------------------------------------------------------
# オプション定義
#------------------------------------------------------------------------------
option(DirectX "Enable DirectX RHI" ON)
option(Vulkan "Enable Vulkan RHI" OFF)
option(UnitTest "Enable UnitTest" ON)

#------------------------------------------------------------------------------
# ソリューション設定
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)                                          # C++17を使用
set(CMAKE_CXX_STANDARD_REQUIRED ON)                                 # 古いSTLにフォールバックしない
set(CMAKE_CXX_EXTENSIONS OFF)                                       # GNU拡張オフ
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" )                     # DebugとReleaseのみ使用(MinSizeRel等を使わない)
set(CMAKE_DEBUG_POSTFIX "")                                         # 

add_definitions(-D_UNICODE -DUNICODE)                               # 文字リテラルをユニコードとする
set_property(GLOBAL PROPERTY USE_FOLDERS ON)                        # フィルタを使用する
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")    # CMake関係のプロジェクトを_CMakeにまとめる

#------------------------------------------------------------------------------
# パス設定
#------------------------------------------------------------------------------
set(OCTBIT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(OCTBIT_SOURCE_PATH "${OCTBIT_ROOT_PATH}/Source")
set(OCTBIT_THIRD_PARTY_PATH "${OCTBIT_SOURCE_PATH}/ThirdParty")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build/bin")

set(OB_CONFIGURATION "Debug")

#------------------------------------------------------------------------------
# プラットフォームごとの設定
#------------------------------------------------------------------------------

# 警告レベルを最大にする
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W4)
else()
    #add_compile_options(-Wno-everything -Werror)
endif()

# 警告抑制
add_compile_options(/wd26495)   # メンバの未初期化
add_compile_options(/wd26451)   # 算術オーバーフロー


#------------------------------------------------------------------------------
# ThirdPartyオプション設定
#------------------------------------------------------------------------------
# DirectXTex
set(BUILD_TOOLS OFF)
# GoogleTest
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST CACHE BOOL OFF "")
if(WIN32)
  set(gtest_force_shared_crt ON CACHE BOOL "")
endif()
set(FG_BUILD_TEST OFF)

# glslang
set(BUILD_SHARED_LIBS OFF)

# assimp
set(ASSIMP_BUILD_TESTS FALSE)
set(P2T_STATIC_EXPORTS TRUE)

#------------------------------------------------------------------------------
# ThirdPartyライブラリ
#------------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(GSL GIT_REPOSITORY https://github.com/microsoft/GSL.git GIT_TAG "v4.0.0")
FetchContent_Declare(utf8cpp GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git GIT_TAG "v4.0.5")
FetchContent_Declare(magic_enum GIT_REPOSITORY https://github.com/Neargye/magic_enum.git GIT_TAG "v0.8.0")
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG "v1.15.2")
FetchContent_Declare(FrameGraph GIT_REPOSITORY https://github.com/skaarj1989/FrameGraph.git)
FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git "v5.2.0")
FetchContent_MakeAvailable(GSL utf8cpp magic_enum googletest FrameGraph assimp)

if(DirectX)
    FetchContent_Declare(DirectXTex GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git GIT_TAG "sept2021")
    FetchContent_MakeAvailable(DirectXTex)
endif()

if(Vulkan)
    # FetchContent_Declare(glslang GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git GIT_TAG "vulkan-1.1-rc9")
    # FetchContent_Declare(spirv GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git GIT_TAG "sdk-1.3.231.1")
    # FetchContent_MakeAvailable(glslang spirv)
endif()

#------------------------------------------------------------------------------
# 汎用機能
#------------------------------------------------------------------------------
include("ThirdParty.cmake")
include("Source/CMake/Common.cmake")


#------------------------------------------------------------------------------
# プロジェクト読み込み
#------------------------------------------------------------------------------

add_definitions(/Zc:__cplusplus)                                    # __cplusplusを使えるようにする
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")            # UTF-8を強制する

# Framework
add_subdirectory ("Source/Framework/Core")
add_subdirectory ("Source/Framework/Platform")
add_subdirectory ("Source/Framework/Input")
add_subdirectory ("Source/Framework/RHI")
add_subdirectory ("Source/Framework/Graphics")
add_subdirectory ("Source/Framework/Model")
#add_subdirectory ("Source/Framework/Asset")
add_subdirectory ("Source/Framework/Debug")
add_subdirectory ("Source/Framework/Engine")
add_subdirectory ("Source/Framework/Asset")
add_subdirectory ("Source/Framework/Editor")

# Plugins
if(WIN32)
    if(DirectX)
        add_subdirectory ("Source/Plugins/DirectX12RHI")
    endif()
    if(Vulkan)
        #add_subdirectory ("Source/Plugins/VulkanRHI")
    endif()
endif()

add_subdirectory ("Source/Plugins/ImGui")

# Test
if(UnitTest)
    enable_testing()
    include(GoogleTest)
    add_subdirectory ("Source/Test/Framework/Core")
    add_subdirectory ("Source/Test/Framework/Platform")
    add_subdirectory ("Source/Test/Framework/RHI")
    add_subdirectory ("Source/Test/Framework/Graphics")
    add_subdirectory ("Source/Test/Framework/Engine")
endif()

# Application
add_subdirectory ("Source/Application")
cmake_minimum_required (VERSION 3.15)
project ("OctbitEngine")

#------------------------------------------------------------------------------
# ソリューション設定
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)                      # C++17を使用
set(CMAKE_CXX_STANDARD_REQUIRED ON)             # 古いSTLにフォールバックしない
set(CMAKE_CXX_EXTENSIONS OFF)                   # GNU拡張オフ
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" ) # DebugとReleaseのみ使用(MinSizeRel等を使わない)
set(CMAKE_DEBUG_POSTFIX "")                     # 

add_definitions(-D_UNICODE -DUNICODE)           # 文字リテラルをユニコードとする
add_definitions(/Zc:__cplusplus)                # __cplusplusを使えるようにする
set_property(GLOBAL PROPERTY USE_FOLDERS ON)    # フィルタを使用する
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")    # CMake関係のプロジェクトを_CMakeにまとめる


#------------------------------------------------------------------------------
# パス設定
#------------------------------------------------------------------------------
set(OCTBIT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(OCTBIT_SOURCE_PATH "${OCTBIT_ROOT_PATH}/Source")
set(OCTBIT_INCLUDE_PATH "${OCTBIT_ROOT_PATH}/Source/Public")
set(OCTBIT_FRAMEWORK_PATH "${OCTBIT_SOURCE_PATH}/Framework")
set(OCTBIT_THIRD_PARTY_PATH "${OCTBIT_SOURCE_PATH}/ThirdParty")
set(OCTBIT_DLL_PATH "${OCTBIT_ROOT_PATH}/Build/Libray")

set(OB_CONFIGURATION "Debug")

#------------------------------------------------------------------------------
# プラットフォームごとの設定
#------------------------------------------------------------------------------

# 警告レベルを最大にする
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W4)
endif()

# 警告抑制
add_compile_options(/wd26495)


#------------------------------------------------------------------------------
# ThirdPartyオプション設定
#------------------------------------------------------------------------------
set(build_tests OFF)
set(build_namespace_tests OFF)
set(TESTS OFF)
set(EXTENSIONS OFF)
set(VERBOSE_CONFIG OFF)
set(MEMORY_LEAK_DETECTION OFF)

#------------------------------------------------------------------------------
# ThirdPartyライブラリ
#------------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG "8.1.1")
FetchContent_Declare(GSL GIT_REPOSITORY https://github.com/microsoft/GSL.git GIT_TAG "v4.0.0")
FetchContent_Declare(utf8cpp GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git GIT_TAG "v3.2.1")
FetchContent_Declare(magic_enum GIT_REPOSITORY https://github.com/Neargye/magic_enum.git GIT_TAG "v0.8.0")
FetchContent_Declare(CppUTest GIT_REPOSITORY https://github.com/cpputest/cpputest.git GIT_TAG "v4.0")
FetchContent_MakeAvailable(fmt GSL utf8cpp magic_enum cpputest)

if(WIN32)
    FetchContent_Declare(DirectXTex GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git GIT_TAG "sept2021")
    FetchContent_MakeAvailable(DirectXTex)
endif()

include(ThirdParty.cmake)


#------------------------------------------------------------------------------
# プロジェクト読み込み
#------------------------------------------------------------------------------
include(Common.cmake)

# Framework
add_subdirectory ("Source/Framework/Singleton")
add_subdirectory ("Source/Framework/Core")
add_subdirectory ("Source/Framework/Platform")
add_subdirectory ("Source/Framework/Graphic")
add_subdirectory ("Source/Framework/Rendering")
add_subdirectory ("Source/Framework/Input")
add_subdirectory ("Source/Framework/Engine")
add_subdirectory ("Source/Framework/Model")

# Plugins
if(WIN32)
    add_subdirectory ("Source/Plugins/GraphicDirectX12")
endif()
#add_subdirectory ("Source/Public/Plugins/GraphicVulkan")

# Test
add_subdirectory ("Source/Test/Framework/Core")

# Application
add_subdirectory ("Source/Application")

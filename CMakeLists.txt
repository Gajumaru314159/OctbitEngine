cmake_minimum_required (VERSION 3.15)
project ("OctbitEngine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")
add_definitions(-D_UNICODE -DUNICODE)
add_definitions(/Zc:__cplusplus)

#------------------------------------------------------------------------------
# options
# option(変数名 "説明" ON/OFF)
#------------------------------------------------------------------------------

set(CMAKE_DEBUG_POSTFIX "")
include(Common.cmake)

# パス設定
set(OCTBIT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(OCTBIT_SOURCE_PATH "${OCTBIT_ROOT_PATH}/Sources")
set(OCTBIT_FRAMEWORK_PATH "${OCTBIT_SOURCE_PATH}/Framework")
set(OCTBIT_THIRD_PARTY_PATH "${OCTBIT_SOURCE_PATH}/ThirdParty")
set(OCTBIT_DLL_PATH "${OCTBIT_ROOT_PATH}/Build/Libray")

set(OB_CONFIGURATION "Debug")

# 警告レベルを最大にする
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W4)
endif()
add_compile_options(/wd26495)

set(build_tests OFF)

# 警告抑制
set(build_namespace_tests OFF)  # 未初期化の変数

# 外部ライブラリ
include(FetchContent)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG "8.1.1")
FetchContent_Declare(GSL GIT_REPOSITORY https://github.com/microsoft/GSL.git GIT_TAG "v4.0.0")
FetchContent_Declare(utf8cpp GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git GIT_TAG "v3.2.1")
FetchContent_Declare(magic_enum GIT_REPOSITORY https://github.com/Neargye/magic_enum.git GIT_TAG "v0.8.0")
FetchContent_Declare(iutest  GIT_REPOSITORY https://github.com/srz-zumix/iutest.git GIT_TAG "v1.17.1")
FetchContent_MakeAvailable(fmt GSL utf8cpp magic_enum iutest)


set_target_properties(fmt PROPERTIES FOLDER "ThirdParty")
set_target_properties(GSL PROPERTIES FOLDER "ThirdParty")
set_target_properties(utf8cpp PROPERTIES FOLDER "ThirdParty")
set_target_properties(magic_enum PROPERTIES FOLDER "ThirdParty")
set_target_properties(iutest PROPERTIES FOLDER "ThirdParty")
set_target_properties(iutest_main PROPERTIES FOLDER "ThirdParty")
set_target_properties(iutest_sample PROPERTIES FOLDER "ThirdParty")
set_property(TARGET fmt PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET GSL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET utf8cpp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET magic_enum PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET iutest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(WIN32)
    FetchContent_Declare(DirectXTex GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git GIT_TAG "sept2021")
    FetchContent_MakeAvailable(DirectXTex)
    
    set_target_properties(DirectXTex PROPERTIES FOLDER "ThirdParty")
    set_target_properties(texconv PROPERTIES FOLDER "ThirdParty")
    set_target_properties(texdiag PROPERTIES FOLDER "ThirdParty")
    set_target_properties(texassemble PROPERTIES FOLDER "ThirdParty")
    set_property(TARGET DirectXTex PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


# Frameworkプロジェクト読み込み
add_subdirectory ("Sources/Framework/Singleton")
add_subdirectory ("Sources/Framework/Core")
add_subdirectory ("Sources/Framework/Platform")
add_subdirectory ("Sources/Framework/Graphic")
#add_subdirectory ("Sources/Framework/Rendering")
add_subdirectory ("Sources/Framework/Input")
#add_subdirectory ("Sources/Framework/Engine")
#add_subdirectory ("Sources/Framework/Model")

# Pluginsプロジェクト読み込み
if(WIN32)
    add_subdirectory ("Sources/Plugins/GraphicDirectX12")
endif()
#add_subdirectory ("Sources/Plugins/GraphicVulkan")

# テストプロジェクト読み込み
add_subdirectory ("Sources/Test/Framework/Core")

# アプリケーションプロジェクト読み込み
add_subdirectory ("Sources/Application")

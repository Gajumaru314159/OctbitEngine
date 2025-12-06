cmake_minimum_required (VERSION 3.15)
project ("OctbitEngine")

#------------------------------------------------------------------------------
# オプション定義
#------------------------------------------------------------------------------
option(OCTBIT_DIRECTX "Enable DirectX RHI" ON)
option(OCTBIT_VULKAN "Enable Vulkan RHI" ON)
option(OCTBIT_TEST "Enable UnitTest" ON)

#------------------------------------------------------------------------------
# パス設定
#------------------------------------------------------------------------------
set(OCTBIT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(OCTBIT_SOURCE_PATH "${OCTBIT_ROOT_PATH}/Engine/Source")
set(OCTBIT_THIRD_PARTY_PATH "${OCTBIT_SOURCE_PATH}/ThirdParty")
set(OCTBIT_SAMPLE_PROJECT_PATH "${OCTBIT_SOURCE_PATH}/Sample/Sandbox")

#------------------------------------------------------------------------------
# ソリューション設定
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)                                                  # C++20を使用
set(CMAKE_CXX_STANDARD_REQUIRED ON)                                         # 古いSTLにフォールバックしない
set(CMAKE_CXX_EXTENSIONS OFF)                                               # GNU拡張オフ
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" )                             # DebugとReleaseのみ使用(MinSizeRel等を使わない)
set(CMAKE_DEBUG_POSTFIX "")                                                 #
set_property(GLOBAL PROPERTY USE_FOLDERS ON)                                # フィルタを使用する
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")            # CMake関係のプロジェクトを_CMakeにまとめる
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build/Bin")   # ライブラリの出力先
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build/Bin")   # 実行ファイルの出力先

#------------------------------------------------------------------------------
# ThirdPartyオプション設定
#------------------------------------------------------------------------------
# mimalloc
set(MI_BUILD_SHARED OFF)
set(MI_BUILD_TESTS OFF)
set(MI_MALLOC_OVERRIDE OFF)
set(MI_USE_CXX ON)
# tracy
#set(TRACY_ENABLE ON)
#set(TRACY_ON_DEMAND ON)
#set(TRACY_DELAYED_INIT ON)
#set(TRACY_MANUAL_LIFETIME ON)
#set(TRACY_DBGHELP_LOCK DbgHelp)
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
FetchContent_Declare(mimalloc GIT_REPOSITORY https://github.com/microsoft/mimalloc.git GIT_TAG "v3.1.5")
FetchContent_Declare(tracy GIT_REPOSITORY https://github.com/wolfpld/tracy GIT_TAG "v0.13.0")
FetchContent_Declare(GSL GIT_REPOSITORY https://github.com/microsoft/GSL.git GIT_TAG "v4.0.0")
FetchContent_Declare(utf8cpp GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git GIT_TAG "v4.0.5")
FetchContent_Declare(magic_enum GIT_REPOSITORY https://github.com/Neargye/magic_enum.git GIT_TAG "v0.8.0")
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG "v1.15.2")
FetchContent_Declare(FrameGraph GIT_REPOSITORY https://github.com/skaarj1989/FrameGraph.git)
FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git "v5.2.0")
FetchContent_MakeAvailable(mimalloc tracy GSL utf8cpp magic_enum googletest FrameGraph assimp)

if(OCTBIT_DIRECTX AND WIN32)
    FetchContent_Declare(DirectXTex GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git GIT_TAG "sept2023")
    FetchContent_MakeAvailable(DirectXTex)
endif()

if(OCTBIT_VULKAN)
    #FetchContent_Declare(spirv GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git GIT_TAG "sdk-1.3.231.1")
    #FetchContent_MakeAvailable(spirv)
endif()

if(UNIX)
    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git "3.4")
    FetchContent_MakeAvailable(glfw)
endif()

#------------------------------------------------------------------------------
# プラットフォームごとの設定
#------------------------------------------------------------------------------

if(MSVC)
    # 警告設定
    add_definitions(/W4)            # 警告レベルを最大にする
    add_compile_options(/wd26495)   # メンバの未初期化
    add_compile_options(/wd26451)   # 算術オーバーフロー
    add_compile_options(/wd4458)    # 'identifier' を宣言すると、クラス メンバーが隠蔽されます
    add_compile_options(/wd4456)    # 'identifier' を宣言すると、以前のローカル宣言が隠蔽されます
    add_compile_options(/wd4201)    # 非標準の拡張機能が使用されています: 無名の構造体または共用体です
    add_compile_options(/wd4101)    # 'identifier': 参照されていないローカル変数
    add_compile_options(/wd4100)    # 仮パラメーターが関数の本体で参照されていません。 未参照のパラメーターは無視されます。
    add_compile_options(/wd4267)    # 'var': 'size_t' から 'type' への変換です。データが失われる可能性があります。
    add_compile_options(/wd4244)    # 'argument' : 'type1' から 'type2' への変換です。データが失われる可能性があります
    add_compile_options(/wd4702)    # unreachable code

    # 文字コード設定
    add_compile_options("/utf-8")   # UTF-8を強制する
    add_definitions(-D_UNICODE)     # 文字リテラルをユニコードとする
    add_definitions(-DUNICODE)      # 文字リテラルをユニコードとする

    # __cplusplus有効化
    add_definitions(/Zc:__cplusplus)# __cplusplusを使えるようにする

endif()

if(CLANG OR GCC)
    # 警告設定
    add_compile_options(-Wno-everything -Werror -Wno-invalid-constexpr)

    # 文字コード設定
    add_compile_options(-utf-8)

    # NDEBUG設定
    add_compile_options(-DNDEBUG)

endif()

#------------------------------------------------------------------------------
# 汎用機能
#------------------------------------------------------------------------------
include("Engine/Source/CMake/ThirdParty.cmake")
include("Engine/Source/CMake/Common.cmake")


#------------------------------------------------------------------------------
# GoogleTestを有効化
#------------------------------------------------------------------------------
if(OCTBIT_TEST)
    enable_testing()
    include(GoogleTest)
endif()


#------------------------------------------------------------------------------
# プロジェクト読み込み
#------------------------------------------------------------------------------
add_subdirectory("Engine/Source")
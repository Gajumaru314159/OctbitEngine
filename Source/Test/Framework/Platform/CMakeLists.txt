cmake_minimum_required (VERSION 3.15)
project (Platform-test)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "PlatformTest")
enable_testing()
add_executable(${PROJECT_NAME} ${out_sources})

SET_PCH("PCH/stdafx.h")

#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PUBLIC CPPUTEST_MEM_LEAK_DETECTION_DISABLED)

# インクルードディレクトリ設定
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(${PROJECT_NAME} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(public_libs Core Platform GTest::gtest_main)
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs})

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Test/Framework")

# GoogleTest
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
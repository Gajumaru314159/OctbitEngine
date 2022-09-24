cmake_minimum_required (VERSION 3.15)
project (Core)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Framework/Core")
add_library(${PROJECT_NAME} STATIC ${out_sources})

#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_CORE_BUILD)

# インクルードディレクトリ設定
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 依存ライブラリ設定
set(public_libs
    fmt
    GSL
    utf8cpp
    magic_enum
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs})

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")
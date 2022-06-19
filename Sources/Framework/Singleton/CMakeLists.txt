cmake_minimum_required (VERSION 3.15)
project (Singleton)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------

LOAD_FILES(out_sources)
add_library(${PROJECT_NAME} SHARED ${out_sources})


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_SINGLETON_BUILD)

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    "${OCTBIT_SOURCE_PATH}"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")

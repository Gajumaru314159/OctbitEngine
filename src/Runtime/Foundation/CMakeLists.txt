cmake_minimum_required (VERSION 3.8)
project (Foundation)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------

LOAD_FILES(out_sources)
add_library(${PROJECT_NAME} STATIC ${out_sources} "Foundation.h" ${gsl_sources})

#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    "${OCTBIT_SOURCE_PATH}"
    "${OCTBIT_EXTERNAL_PATH}/gsl/include"
    "${OCTBIT_EXTERNAL_PATH}/fmt/include"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    "${OCTBIT_EXTERNAL_PATH}/fmt/lib/${OB_CONFIGURATION}/fmt.lib"
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Runtime")

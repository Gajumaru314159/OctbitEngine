cmake_minimum_required (VERSION 3.8)
project (Core)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources)
add_library(${PROJECT_NAME} STATIC ${out_sources} ${gsl_sources})

#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR 
    "${OCTBIT_SOURCE_PATH}"
    "${OCTBIT_THIRD_PARTY_PATH}/gsl/include"
    "${OCTBIT_THIRD_PARTY_PATH}/fmt/include"
    "${OCTBIT_THIRD_PARTY_PATH}/MagicEnum/include"
    "${OCTBIT_THIRD_PARTY_PATH}/utfcpp/include"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    "${OCTBIT_THIRD_PARTY_PATH}/fmt/lib/${OB_CONFIGURATION}/fmt.lib"
    Singleton
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")
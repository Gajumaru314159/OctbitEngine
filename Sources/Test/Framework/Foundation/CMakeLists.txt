﻿cmake_minimum_required (VERSION 3.8)
project (Foundation-test)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources)
add_executable(${PROJECT_NAME} ${out_sources} "main.cpp")

SET_PCH("PCH/stdafx.h" "PCH/stdafx.cpp")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    "${OCTBIT_THIRD_PARTY_PATH}/iutest/include"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Foundation
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Test/Framework")
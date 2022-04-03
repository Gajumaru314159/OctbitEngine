cmake_minimum_required (VERSION 3.8)
project (Singleton)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------

LOAD_FILES(out_sources)
add_library(${PROJECT_NAME} SHARED ${out_sources})

SET_PCH("PCH/stdafx.h" "PCH/stdafx.cpp")

#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_SINGLETON_BUILD)

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Foundation
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")

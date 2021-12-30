cmake_minimum_required (VERSION 3.8)
project (GraphicD3D12)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources)
add_library(${PROJECT_NAME} SHARED ${out_sources} "stdafx.h" "stdafx.cpp")

ADD_FORCE_INCLUDE("Runtime/Foundation/Foundation.h")
ADD_FORCE_INCLUDE("Runtime/GraphicD3D12/stdafx.h")

#target_precompile_headers(${PROJECT_NAME} PRIVATE "stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    "${OCTBIT_ROOT_PATH}/src"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    GraphicBase
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Runtime/Graphic")


target_compile_definitions(${PROJECT_NAME} PRIVATE OB_BUILD_DLL)
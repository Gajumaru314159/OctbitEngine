cmake_minimum_required (VERSION 3.15)
project (RHI)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Framework/RHI")
add_library(${PROJECT_NAME} STATIC ${out_sources})

SET_PCH("Framework/RHI/PCH/stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public" "${OCTBIT_THIRD_PARTY_PATH}/stb/include")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Core
    Platform
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_LIB})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")
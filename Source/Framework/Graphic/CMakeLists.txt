cmake_minimum_required (VERSION 3.15)
project (Graphic)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Framework/Graphic")
add_library(${PROJECT_NAME} STATIC ${out_sources})

SET_PCH("Framework/Graphic/PCH/stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public" "${OCTBIT_THIRD_PARTY_PATH}/stb/include")
set(protected_includes "${CMAKE_CURRENT_SOURCE_DIR}/Protected")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private" "${CMAKE_CURRENT_SOURCE_DIR}/Protected")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} INTERFACE ${protected_includes} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Core
    Platform
    Engine
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_LIB})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")
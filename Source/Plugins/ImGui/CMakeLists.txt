cmake_minimum_required (VERSION 3.15)
project (ImGui)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Plugins/ImGui")
add_library(${PROJECT_NAME} STATIC ${out_sources})

# SET_PCH("Plugins/ImGui/PCH/stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# インクルードディレクトリ設定
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private" 
"${CMAKE_CURRENT_SOURCE_DIR}/Public/Plugins/ImGui/Library/misc/freetype"
"${CMAKE_CURRENT_SOURCE_DIR}/Public/Plugins/ImGui/Library")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(public_libs 
    Core
    Engine
    Platform
    RHI
    Input
)
set(private_libs 
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Plugins/ImGui")
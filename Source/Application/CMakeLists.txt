cmake_minimum_required (VERSION 3.15)
project (Application)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Application")
add_executable(${PROJECT_NAME} ${out_sources})

SET_PCH("PCH/stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_PLATFORM_BUILD)

# インクルードディレクトリ設定
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public" "${CMAKE_CURRENT_SOURCE_DIR}/Source" "${OCTBIT_THIRD_PARTY_PATH}/ObjLoader/include")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(public_libs 
    Core
    Platform
    Graphic
    GraphicDirectX12
    Input
    Engine
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs})

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Application")
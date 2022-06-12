cmake_minimum_required (VERSION 3.21)
project (GraphicDirectX12)
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

target_compile_definitions(${PROJECT_NAME} PRIVATE OB_GRAPHIC_DIRECTX12_BUILD)

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    "${OCTBIT_SOURCE_PATH}"
    "${OCTBIT_THIRD_PARTY_PATH}/PIX/include"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Graphic
    DirectXTex
    "d3d12.lib"
    "dxgi.lib"
    "dxguid.lib"
    "d3dcompiler.lib"
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Plugins/Graphic")

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_BUILD_DLL)

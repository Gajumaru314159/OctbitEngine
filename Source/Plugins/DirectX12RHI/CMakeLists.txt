cmake_minimum_required (VERSION 3.15)
project (DirectX12RHI)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Plugins/DirectX12RHI")
add_library(${PROJECT_NAME} STATIC ${out_sources})

SET_PCH("Plugins/DirectX12RHI/PCH/stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_GRAPHIC_DIRECTX12_BUILD)
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_BUILD_DLL)

# インクルードディレクトリ設定
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private" "${OCTBIT_THIRD_PARTY_PATH}/PIX/include")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(public_libs 
    Core
    RHI
)
set(private_libs 
    DirectXTex
    d3d12.lib
    dxgi.lib
    dxguid.lib
    d3dcompiler.lib
    "${OCTBIT_THIRD_PARTY_PATH}/PIX/bin/WinPixEventRuntime.lib"
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Plugins/RHI")
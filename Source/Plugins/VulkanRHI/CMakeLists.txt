cmake_minimum_required (VERSION 3.22)
project (VulkanRHI)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Plugins/VulkanRHI")
add_library(${PROJECT_NAME} STATIC ${out_sources})

SET_PCH("Plugins/VulkanRHI/PCH/stdafx.h")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# インクルードディレクトリ設定
set(public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public" "${OCTBIT_THIRD_PARTY_PATH}/dxc/inc")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private" "$ENV{VK_SDK_PATH}/Include" "${OCTBIT_THIRD_PARTY_PATH}/stb/include")
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 追加の依存ライブラリ
set(public_libs 
    Core
    RHI
)
set(private_libs     
    "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/dxc/lib/x64/dxcompiler.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/dxc/lib/x64/dxil.lib"
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})
target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{VULKAN_SDK}/Bin")

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Plugins/RHI")
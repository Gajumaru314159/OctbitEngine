cmake_minimum_required (VERSION 3.15)
project (GraphicVulkan)
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

# DLL用define定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_GRAPHIC_VULKAN_BUILD)

# ThirdPartyライブラリ
find_package(Vulkan REQUIRED)


# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    ${OCTBIT_SOURCE_PATH}
    ${Vulkan_INCLUDE_DIRS}
    #"$ENV{VULKAN_SDK}/Include"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Graphic 
    ${Vulkan_LIBRARIES}
    #"$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Plugins/Graphic")

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_BUILD_DLL)

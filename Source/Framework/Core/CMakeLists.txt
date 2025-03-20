cmake_minimum_required (VERSION 3.15)
project (Core)

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources "Framework/Core")
add_library(${PROJECT_NAME} STATIC ${out_sources})

#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 定義
target_compile_definitions(${PROJECT_NAME} PRIVATE OB_CORE_BUILD)

# インクルードディレクトリ設定
set(public_includes 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public"
    "${OCTBIT_THIRD_PARTY_PATH}/nlohmann"
)
set(private_includes
    "${CMAKE_CURRENT_SOURCE_DIR}/Private"
    "${OCTBIT_THIRD_PARTY_PATH}/GDeflate"
)
if (WIN32)
    list(APPEND private_includes "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/include")
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})

# 依存ライブラリ設定
set(public_libs
    GSL
    utf8cpp
    magic_enum
)
set(private_libs    
    "${OCTBIT_THIRD_PARTY_PATH}/GDeflate/GDeflate.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/GDeflate/deflate.lib"
)
if (WIN32)
    list(APPEND private_libs "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/lib/x64/dstorage.lib")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})

# ランタイムライブラリ設定
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# フィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")
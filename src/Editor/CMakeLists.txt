cmake_minimum_required (VERSION 3.8)
project ("OctbitCore")
set(OB_INCLUDE_DIR ${OCTBIT_ROOT_PATH}/include)


#------------------------------------------------------------------------------
# 関数定義
#------------------------------------------------------------------------------
function(ob_load_files dir_name)
    # ファイル検索
    file(
        GLOB
        OB_SOURCES_LOCAL
        "${OB_INCLUDE_DIR}/OctbitCore/${dir_name}/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/${dir_name}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/${dir_name}/*.hpp")
    # フィルタ設定 
    source_group("${dir_name}" FILES ${OB_SOURCES_LOCAL})
    # 読み込みファイルの結合
    set(OB_SOURCES ${OB_SOURCES} ${OB_SOURCES_LOCAL} PARENT_SCOPE)
endfunction()


#------------------------------------------------------------------------------
# ファイルの読み込み
#------------------------------------------------------------------------------
ob_load_files(Base)
ob_load_files(Main)
ob_load_files(Debug)
ob_load_files(File)
ob_load_files(File/JSON)
ob_load_files(File/XML)
ob_load_files(Graphic)
ob_load_files(Math)
ob_load_files(Math/Geometory)
ob_load_files(Memory)
ob_load_files(Memory/Heap)
ob_load_files(Misc)
ob_load_files(Network)
ob_load_files(Template)
ob_load_files(System/Thread)


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------



# プロジェクトにソースを追加
add_library(${PROJECT_NAME} STATIC ${OB_SOURCES})

# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR
    ${OB_INCLUDE_DIR}
    ${OCTBIT_ROOT_PATH}/external/EASTL/include
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})


# ライブラリディレクトリの指定
set(OB_PROJECT_LIB_DIR
    ${OCTBIT_ROOT_PATH}/external/EASTL/include
)
link_directories(${OB_PROJECT_LIB_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    "${OCTBIT_ROOT_PATH}/external/EASTL//lib/Debug/EASTL.lib"
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OB_PROJECT_INCLUDE_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Runtime")


#------------------------------------------------------------------------------
# 
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
#target_include_directories(${PROJECT_NAME} PUBLIC ${OB_CORE_INCLUDE_DIR})
# 追加の依存ライブラリ
# target_link_libraries(${PROJECT_NAME} PRIVATE ${LNOB_LINK_LIBS})

# プロジェクトのフィルタ設定
#set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Octbit")
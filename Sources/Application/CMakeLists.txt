cmake_minimum_required (VERSION 3.8)
project (Application)
message("Load project [${PROJECT_NAME}]")

#------------------------------------------------------------------------------
# ソースコードの登録
#------------------------------------------------------------------------------
LOAD_FILES(out_sources)
add_executable(${PROJECT_NAME} ${out_sources})

SET_PCH("PCH/stdafx.h" "PCH/stdafx.cpp")


#------------------------------------------------------------------------------
# プロジェクト設定
#------------------------------------------------------------------------------

# 追加のインクルードディレクトリ
#set(OB_PROJECT_INCLUDE_DIR
#    "${OCTBIT_ROOT_PATH}/src/Runtime"
#)
#target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    Core
    #Platform
    #Graphic
    #GraphicDirectX12
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OB_PROJECT_INCLUDE_LIB})

# プロジェクトのフィルタ設定
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Application")


# カスタムコマンド
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GraphicDirectX12> $<TARGET_FILE_DIR:Application>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Singleton> $<TARGET_FILE_DIR:Application>
    )
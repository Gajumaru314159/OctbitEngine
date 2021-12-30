cmake_minimum_required (VERSION 3.8)

#------------------------------------------------------------------------------
# options
#------------------------------------------------------------------------------

#set(CMAKE_CXX_STANDARD 17)


#------------------------------------------------------------------------------
# サブフォルダの列挙
#------------------------------------------------------------------------------
FUNCTION(SUBDIRLIST result relative_path)
  FILE(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${relative_path})
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
	  LIST(APPEND dirlist ${child})
	  SUBDIRLIST(subdirs "${child}/*")
	  LIST(APPEND dirlist ${subdirs})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# カレントとディレクトリ以下のソースファイルを読み込み
#------------------------------------------------------------------------------
MACRO(LOAD_FILES result)
	SET(dirs "")
	SUBDIRLIST(dirs "*")
	
	SET(all_source "")
	FOREACH(dir ${dirs})
		# ファイル検索
		FILE(
			GLOB
			sources
			RELATIVE
			${CMAKE_CURRENT_SOURCE_DIR}
			"${dir}/*.h"
			"${dir}/*.cpp")
		SET(all_source ${all_source} ${sources})
		SOURCE_GROUP("${dir}" FILES ${sources})
	ENDFOREACH()
	set(${result} ${all_source})
ENDMACRO()

#------------------------------------------------------------------------------
# すべてのファイルにインクルードさせるファイルを追加
#------------------------------------------------------------------------------
FUNCTION(ADD_FORCE_INCLUDE path)
	if(MSVC)
		add_definitions(/FI"${path}")
	else()
		# GCC or Clang
		add_definitions(-include ${path})
	endif()
ENDFUNCTION()
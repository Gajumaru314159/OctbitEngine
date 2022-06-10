cmake_minimum_required (VERSION 3.21)

#------------------------------------------------------------------------------
# サブフォルダの列挙
#------------------------------------------------------------------------------
FUNCTION(SUBDIRLIST result relative_path)
  FILE(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${relative_path})
  SET(dirlist "")
  FOREACH(child ${children})
	IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
	  IF(NOT child MATCHES "_$")
		LIST(APPEND dirlist ${child})
		SUBDIRLIST(subdirs "${child}/*")
		LIST(APPEND dirlist ${subdirs})
	  ENDIF()
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# カレントとディレクトリ以下のソースファイルを読み込み
#------------------------------------------------------------------------------
MACRO(LOAD_FILES result)
	get_filename_component(project_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY ${})

	SET(dirs "./")
	SUBDIRLIST(dirs "*")
	SET(dirs ${dirs} "./")
	
	SET(all_source "")
	FOREACH(dir ${dirs})
		# ファイル検索
		FILE(
			GLOB
			sources
			RELATIVE
			${CMAKE_CURRENT_SOURCE_DIR}
			"${dir}/*.natvis"
			"${dir}/*.h"
			"${dir}/*.cpp"
			"${dir}/*.md")
		SET(all_source ${all_source} ${sources})
		if(${dir} STREQUAL "./")
			SOURCE_GROUP("Sources/" FILES ${sources})
		else()
			SOURCE_GROUP("Sources/${dir}" FILES ${sources})
		endif()
	ENDFOREACH()
	set(${result} ${all_source})
ENDMACRO()

#------------------------------------------------------------------------------
# すべてのファイルにインクルードさせるファイルを追加
#------------------------------------------------------------------------------
FUNCTION(SET_PCH header source)
	if(MSVC)
		# プリコンパイル済みヘッダの使用(/Yu)を全体に設定
		add_definitions(/FI${CMAKE_CURRENT_SOURCE_DIR}/${header})
		#set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${source} PROPERTIES COMPILE_FLAGS "/Yc")
	else()
		# GCC or Clang
		add_definitions(-include ${CMAKE_CURRENT_SOURCE_DIR}/${header})
	endif()
ENDFUNCTION()
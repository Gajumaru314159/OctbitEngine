cmake_minimum_required (VERSION 3.15)

#------------------------------------------------------------------------------
# サブフォルダの列挙
#------------------------------------------------------------------------------
FUNCTION(SUBDIRLIST_RECLUSIVE result root_path relative_path)
	FILE(GLOB children RELATIVE ${root_path} ${root_path}/${relative_path})
	
	SET(dirlist "")
	FOREACH(child ${children})
		IF(IS_DIRECTORY ${root_path}/${child})
			IF(NOT child MATCHES "_$")
				LIST(APPEND dirlist ${child})
				SUBDIRLIST_RECLUSIVE(subdirs ${root_path} "${child}/*")
				LIST(APPEND dirlist ${subdirs})
			ENDIF()
		ENDIF()
	ENDFOREACH()
	SET(${result} ${dirlist} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# サブフォルダの列挙
#------------------------------------------------------------------------------
FUNCTION(SUBDIRLIST result root_path)
	SET(dirs "")
	SUBDIRLIST_RECLUSIVE(dirs ${root_path} "*")
	SET(${result} ${dirs} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# サブフォルダの列挙
#------------------------------------------------------------------------------
FUNCTION(ADD_SOURCES result filter location)
	SET(root_path "${OCTBIT_SOURCE_PATH}/${filter}/${location}")
	SUBDIRLIST(dirs ${root_path})
	LIST(APPEND dirs ".")
	SET(all_files "")
	FOREACH(dir ${dirs})
	
		SET(abs_dir ${root_path}/${dir})
		# ファイル検索
		FILE(
			GLOB
			files
			ABSOLUTE
			"${abs_dir}/*.natvis"
			"${abs_dir}/*.h"
			"${abs_dir}/*.cpp"
			"${abs_dir}/*.md")
		# ソースリストを結合
		LIST(APPEND all_files ${files})
		# フィルタ設定
		if(${dir} STREQUAL ".")
			SOURCE_GROUP("${filter}/" FILES ${files})
		else()
			SOURCE_GROUP("${filter}/${dir}" FILES ${files})
		endif()
	ENDFOREACH()

	SET(${result} ${all_files} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# カレントとディレクトリ以下のソースファイルを読み込み
#------------------------------------------------------------------------------
FUNCTION(LOAD_FILES result location)
	get_filename_component(project_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY ${})
	
	# フォルダ列挙
	ADD_SOURCES(public_files "Public" ${location})
	ADD_SOURCES(private_files "Private" ${location})
	
	SET(files ${public_files} ${private_files})

	SET(${result} ${files} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# すべてのファイルにインクルードさせるファイルを追加
#------------------------------------------------------------------------------
FUNCTION(SET_PCH header source)
	if(MSVC)
		# プリコンパイル済みヘッダの使用(/Yu)を全体に設定
		add_definitions(/FI${CMAKE_CURRENT_SOURCE_DIR}/${header})
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yu${CMAKE_CURRENT_SOURCE_DIR}/${header}")
		set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${source} PROPERTIES COMPILE_FLAGS "/Yc${CMAKE_CURRENT_SOURCE_DIR}/${header}")
	else()
		# GCC or Clang
		add_definitions(-include ${CMAKE_CURRENT_SOURCE_DIR}/${header})
	endif()
ENDFUNCTION()
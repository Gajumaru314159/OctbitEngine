cmake_minimum_required (VERSION 3.15)

#------------------------------------------------------------------------------
# サブフォルダの列挙
#------------------------------------------------------------------------------
FUNCTION(SUBDIRLIST_RECLUSIVE result root_path relative_path)
	FILE(GLOB children RELATIVE ${root_path} ${root_path}/${relative_path})
	
	SET(dirlist "")
	FOREACH(child ${children})
		IF(IS_DIRECTORY ${root_path}/${child})
			IF(NOT child MATCHES "_.*") # _から始まるディレクトリは除外
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
FUNCTION(ADD_SOURCES result filter root)

	SET(root_path "${CMAKE_CURRENT_SOURCE_DIR}/${filter}/${root}")
	# サブディレクトリを列挙(ルートからの相対)
	SUBDIRLIST(dirs ${root_path})
	# ルートを追加
	LIST(APPEND dirs ".")
	
	# ソースを追加
	SET(all_files "")
	FOREACH(dir ${dirs})
	
		SET(abs_dir ${root_path}/${dir})
		# ファイル検索
		FILE(
			GLOB
			files
			ABSOLUTE
			"${abs_dir}/*.md"
			"${abs_dir}/*.natvis"
			"${abs_dir}/*.h"
			"${abs_dir}/*.cpp")
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
FUNCTION(LOAD_FILES result root)

	message("Load project [${PROJECT_NAME}]")

	# ファイルを列挙
	ADD_SOURCES(public_files "Public" ${root})
	ADD_SOURCES(private_files "Private" ${root})
	ADD_SOURCES(test_files "Source" ".")
	ADD_SOURCES(test_files "Test" "Test")
	ADD_SOURCES(misc_files "Misc" ".")
	
	# PublicとPrivateを結合
	SET(files ${public_files})
	LIST(APPEND files ${private_files})
	LIST(APPEND files ${test_files})
	LIST(APPEND files ${misc_files})
	# resultに格納
	SET(${result} ${files} PARENT_SCOPE)

ENDFUNCTION()

#------------------------------------------------------------------------------
# すべてのファイルにインクルードさせるファイルを追加
#------------------------------------------------------------------------------
FUNCTION(SET_PCH header)
	if(MSVC)
		# プリコンパイル済みヘッダの使用(/Yu)を全体に設定
		add_definitions(/FI${header})
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yu${header}")
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Fp")
	else()
		# GCC or Clang
		add_definitions(-include ${header})
	endif()
ENDFUNCTION()
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
	LIST(APPEND dirs ${root_path})
	SET(${result} ${dirs} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# サブフォルダの列挙
# result      : 結果を格納する変数名
# filter      : 格納するVisualStudioのフィルタ名
# dir_root    : 検索するルートのディレクトリ
#------------------------------------------------------------------------------
FUNCTION(ADD_SOURCES result dir_root filter)

	SET(root_path "${CMAKE_CURRENT_SOURCE_DIR}/${dir_root}")
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
			"${abs_dir}/*.natvis"
			"${abs_dir}/*.h"
			"${abs_dir}/*.cpp")
		# ソースリストを結合
		LIST(APPEND all_files ${files})		
		# フィルタ設定
		if(${dir} STREQUAL ".")
			SOURCE_GROUP("${filter}" FILES ${files})
		else()
			SOURCE_GROUP("${filter}/${dir}" FILES ${files})
		endif()

	ENDFOREACH()
	
	SET(${result} ${all_files} PARENT_SCOPE)
ENDFUNCTION()

#------------------------------------------------------------------------------
# カレントとディレクトリ以下のソースファイルを読み込み
#------------------------------------------------------------------------------
FUNCTION(OB_LOAD_FILES result)

	message("Load project [${PROJECT_NAME}]")

	# ファイルを列挙
	ADD_SOURCES(public_files Public Public)
	ADD_SOURCES(private_files Private Private)
	ADD_SOURCES(src_files Source Source)

	# PublicとPrivateを結合
	SET(files "")
	LIST(APPEND files ${public_files})
	LIST(APPEND files ${private_files})
	LIST(APPEND files ${src_files})

	# resultに格納
	SET(${result} ${files} PARENT_SCOPE)

ENDFUNCTION()

#------------------------------------------------------------------------------
# カレントとディレクトリ以下のソースファイルを読み込み
#------------------------------------------------------------------------------
FUNCTION(OB_LOAD_TEST_FILES result)

	message("Load project [${PROJECT_NAME}]")

	ADD_SOURCES(test_files . Test)

	# PublicとPrivateを結合
	SET(files "")
	LIST(APPEND files ${test_files})

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


#------------------------------------------------------------------------------
# ビルド後にコピーするファイルを追加する
#------------------------------------------------------------------------------
FUNCTION(ob_add_copy_command files)

ENDFUNCTION()

#------------------------------------------------------------------------------
# exeの実行に必要なDLLをコピーする
#------------------------------------------------------------------------------
FUNCTION(COPY_REQUIRED_DLL)
	if(WIN32)
		add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCTBIT_THIRD_PARTY_PATH}/PIX/bin/WinPixEventRuntime.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/bin/x64/dxcompiler.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/bin/x64/dxil.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/bin/x64/dstorage.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/bin/x64/dstoragecore.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	endif()
ENDFUNCTION()

#------------------------------------------------------------------------------
# exeの実行に必要なDLLをコピーする
#------------------------------------------------------------------------------
FUNCTION(SETUP_LIB_COMMON)
	
	# ランタイムライブラリ設定
	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

ENDFUNCTION()

#------------------------------------------------------------------------------
# Frameworkモジュールを追加
# * Public/Private以下のソースファイルを登録
# * Public/Framework/{PROJECT_NAME}/pch.hが存在すればプリコンパイルヘッダに登録
# * ライブラリと追加の依存ディレクトリを登録
# * Frameworkフィルターを設定
#------------------------------------------------------------------------------
FUNCTION(OB_ADD_LIBRARY)	
	
	# ソースコードを登録
	OB_LOAD_FILES(out_sources)
	add_library(${PROJECT_NAME} STATIC ${out_sources})

	# プリコンパイルヘッダを設定
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Public/Framework/${PROJECT_NAME}/pch.h)
		SET_PCH("Framework/${PROJECT_NAME}/pch.h")
	endif()

	# インクルードディレクトリ設定
	list(APPEND public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public")
	list(APPEND private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Private")
	target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})
	
	# 依存ライブラリ設定
	target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})

	# フィルタ設定
	set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Framework")

	# 共通設定
	SETUP_LIB_COMMON()

ENDFUNCTION()

#------------------------------------------------------------------------------
# テストモジュールを追加
#------------------------------------------------------------------------------
FUNCTION(OB_ADD_TEST)	

	LIST(APPEND private_libs GTest::gtest_main)
	
	# ソースコードを登録
	OB_LOAD_TEST_FILES(out_sources)
	add_executable(${PROJECT_NAME} ${out_sources})

	# プリコンパイルヘッダを設定
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
		SET_PCH("pch.h")
	endif()

	# 定義
	target_compile_definitions(${PROJECT_NAME} PUBLIC CPPUTEST_MEM_LEAK_DETECTION_DISABLED)

	# インクルードディレクトリ設定
	list(APPEND private_includes "${CMAKE_CURRENT_SOURCE_DIR}")
	target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})
	
	# 依存ライブラリ設定
	target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})
	
	# ランタイムライブラリ設定
	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

	# フィルタ設定
	set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Test")

	# GoogleTest
	gtest_discover_tests(${PROJECT_NAME})

	# カスタムコマンド
	COPY_REQUIRED_DLL()

ENDFUNCTION()


#------------------------------------------------------------------------------
# アプリケーションを追加
#------------------------------------------------------------------------------
FUNCTION(OB_ADD_APP)	
	
	# ソースコードを登録
	OB_LOAD_FILES(out_sources)
	add_executable(${PROJECT_NAME} ${out_sources})

	# プリコンパイルヘッダを設定
	SET_PCH("pch.h")

	# 定義
	target_compile_definitions(${PROJECT_NAME} PRIVATE OB_PLATFORM_BUILD)

	# インクルードディレクトリ設定
	list(APPEND public_includes "${CMAKE_CURRENT_SOURCE_DIR}/Source")
	target_include_directories(${PROJECT_NAME} PUBLIC ${public_includes} PRIVATE ${private_includes})
	
	# 依存ライブラリ設定
	target_link_libraries(${PROJECT_NAME} PUBLIC ${public_libs} PRIVATE ${private_libs})


	# ランタイムライブラリ設定
	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

	# フィルタ設定
	set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Application")

	# カスタムコマンド
	COPY_REQUIRED_DLL()

ENDFUNCTION()


#------------------------------------------------------------------------------
# テストモジュールを追加する
#------------------------------------------------------------------------------
FUNCTION(ob_add_test_module)
	if(OCTBIT_TEST)
		add_subdirectory("Test")
	endif()
ENDFUNCTION()
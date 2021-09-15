cmake_minimum_required (VERSION 3.8)

#------------------------------------------------------------------------------
# options
#------------------------------------------------------------------------------

#set(CMAKE_CXX_STANDARD 17)


#------------------------------------------------------------------------------
# make lib dir
#------------------------------------------------------------------------------
set(OB_LIB_OUTPUT_LOCAL_DIR "lib/")

if(WIN32)
    if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64
        set(OB_ARCH "x64")
    else()
        set(OB_ARCH "x86")
    endif()
endif()

#------------------------------------------------------------------------------
# pch追加関数
#------------------------------------------------------------------------------
function(ln_add_pch project_name header_file_path source_file_path)

	get_filename_component(header_file_name ${header_file_path} NAME)

	if (MSVC)
		set(ln_compile_flags
			#"/Yu\"${header_file_name}\" /FI\"${header_file_name}\""	# use PCH, ForcedIncludeFiles
			"/Yu\"${header_file_name}\""
		)
		target_compile_options(${project_name} PRIVATE /FI\"${header_file_name}\")
	else()
		# https://github.com/nanoant/CMakePCHCompiler/blob/master/CMakePCHCompiler.cmake

		get_filename_component(result ${header_file_path} ABSOLUTE)

		# force include header
		set(ob_compile_flags "-include \"${result}\"")
	endif()


	# get source files from project (referred LLVM)
	get_property(source_files TARGET ${project_name} PROPERTY SOURCES)

	# set ob_compile_flags to all *.cpp
	foreach (file ${source_files})
		if (${file} MATCHES ".+\\.cpp")
			set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS ${ob_compile_flags})
		endif()
	endforeach()

	# create .pch config
	if (MSVC)
		set_source_files_properties(${source_file_path} PROPERTIES COMPILE_FLAGS "/Yc\"${header_file_name}\"")
	endif()

endfunction()
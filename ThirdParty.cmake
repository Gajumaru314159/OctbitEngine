cmake_minimum_required (VERSION 3.15)

#------------------------------------------------------------------------------
# プロジェクトの列挙
#------------------------------------------------------------------------------
function (_get_all_cmake_targets out_var current_dir)
    get_property(targets DIRECTORY ${current_dir} PROPERTY BUILDSYSTEM_TARGETS)
    get_property(subdirs DIRECTORY ${current_dir} PROPERTY SUBDIRECTORIES)

    foreach(subdir ${subdirs})
        _get_all_cmake_targets(subdir_targets ${subdir})
        list(APPEND targets ${subdir_targets})
    endforeach()

    set(${out_var} ${targets} PARENT_SCOPE)
endfunction()

# ThirdParty共通処理
_get_all_cmake_targets(all_targets ${CMAKE_CURRENT_LIST_DIR})

foreach(target ${all_targets})
	set_target_properties(${target} PROPERTIES FOLDER "ThirdParty")
	set_property(TARGET ${target} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endforeach()
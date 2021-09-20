cmake_minimum_required (VERSION 3.8)
project ("OctbitEngine")

set(CMAKE_CXX_STANDARD 17) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#------------------------------------------------------------------------------
# options
# option(変数名 "説明" ON/OFF)
#------------------------------------------------------------------------------

set(CMAKE_DEBUG_POSTFIX "d")
include(src/BuildCommon.cmake)
include(Common.cmake)

set(OCTBIT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")

set(OCTBIT_INCLUDE_DIRECTORIES
    "${OCTBIT_ROOT_PATH}/include/Runtime")

set(OCTBIT_EXTERNAL_PATH "${OCTBIT_ROOT_PATH}/external")
set(OB_CONFIGURATION "Debug")

if(MSVC)
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_DEBUG            ${CMAKE_C_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_MINSIZEREL       ${CMAKE_C_FLAGS_MINSIZEREL})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELWITHDEBINFO   ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL     ${CMAKE_CXX_FLAGS_MINSIZEREL})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
endif(MSVC)

if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W4)
endif()

if (MSVC)
    #set(OB_COMMON_META_FILES "${CMAKE_CURRENT_LIST_DIR}/tools/VisualStudio/Lumino.natvis")
endif()


# プロジェクト読み込み
add_subdirectory ("src/Runtime/Foundation")
add_subdirectory ("src/Runtime/Core")
#add_subdirectory ("src/Runtime/Graphic")
#add_subdirectory ("src/Runtime/GraphicD3D12")
# add_subdirectory ("src/Runtime/GraphicVulkan")

# テストプロジェクト読み込み
add_subdirectory ("test/Runtime/Foundation")
add_subdirectory ("test/Runtime/Core")

# アプリケーションプロジェクト読み込み
#add_subdirectory ("src/Application")
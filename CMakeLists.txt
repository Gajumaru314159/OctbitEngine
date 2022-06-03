cmake_minimum_required (VERSION 3.8)
project ("OctbitEngine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")
#------------------------------------------------------------------------------
# options
# option(変数名 "説明" ON/OFF)
#------------------------------------------------------------------------------

#set(CMAKE_DEBUG_POSTFIX "d")
include(Common.cmake)

set(OCTBIT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(OCTBIT_SOURCE_PATH "${OCTBIT_ROOT_PATH}/Sources")
set(OCTBIT_FRAMEWORK_PATH "${OCTBIT_SOURCE_PATH}/Framework")
set(OCTBIT_THIRD_PARTY_PATH "${OCTBIT_SOURCE_PATH}/ThirdParty")
set(OCTBIT_DLL_PATH "${OCTBIT_ROOT_PATH}/Build/Libray")

set(OB_CONFIGURATION "Debug")


if(MSVC)
    #set(CMAKE_CXX_FLAGS "/Yu")
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_DEBUG            ${CMAKE_C_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_MINSIZEREL       ${CMAKE_C_FLAGS_MINSIZEREL})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELWITHDEBINFO   ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL     ${CMAKE_CXX_FLAGS_MINSIZEREL})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
endif(MSVC)

add_definitions(-D_UNICODE -DUNICODE)
add_definitions(/Zc:__cplusplus)

if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W4)
endif()

if (MSVC)
endif()



# Frameworkプロジェクト読み込み
add_subdirectory ("Sources/Framework/Singleton")
add_subdirectory ("Sources/Framework/Core")
add_subdirectory ("Sources/Framework/Platform")
add_subdirectory ("Sources/Framework/Graphic")
add_subdirectory ("Sources/Framework/Rendering")

# Pluginsプロジェクト読み込み
add_subdirectory ("Sources/Plugins/GraphicDirectX12")
add_subdirectory ("Sources/Plugins/GraphicVulkan")

# テストプロジェクト読み込み
add_subdirectory ("Sources/Test/Framework/Core")

# アプリケーションプロジェクト読み込み
add_subdirectory ("Sources/Application")

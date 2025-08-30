cmake_minimum_required (VERSION 3.15)
project(Core)

# インクルードディレクトリ設定
set(public_includes 
    "${OCTBIT_THIRD_PARTY_PATH}/nlohmann"
)
set(private_includes
    "${OCTBIT_THIRD_PARTY_PATH}/GDeflate"
)
if (WIN32)
    list(APPEND private_includes "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/include")
endif()


# 依存ライブラリ設定
set(public_libs
    GSL
    utf8cpp
    magic_enum
)
set(private_libs
    mimalloc
    "${OCTBIT_THIRD_PARTY_PATH}/GDeflate/GDeflate.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/GDeflate/deflate.lib"
)
if (WIN32)
    list(APPEND private_libs "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/lib/x64/dstorage.lib")
endif()

message(${public_libs})
OB_ADD_FRAMEWORK_LIBRARY("${public_includes}" "${private_includes}" "${public_libs}" "${private_libs}")
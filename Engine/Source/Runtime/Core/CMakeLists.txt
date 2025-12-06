project(Core)

# 依存ライブラリ
set(public_libs
    GSL
    utf8cpp
    magic_enum
    TracyClient
)
set(private_libs
    mimalloc-static
)
if (WIN32)
    # AsyncFileおよびCompressで使用するライブラリ
    list(APPEND private_libs
        "${OCTBIT_THIRD_PARTY_PATH}/GDeflate/GDeflate.lib"
        "${OCTBIT_THIRD_PARTY_PATH}/GDeflate/deflate.lib"
        "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/lib/x64/dstorage.lib"
    )
endif()
if (UNIX)
    # UUIDで使用するライブラリ
    list(APPEND private_libs uuid stdc++_libbacktrace)
endif()


# 追加のインクルードディレクトリ
set(public_includes 
    "${OCTBIT_THIRD_PARTY_PATH}/nlohmann"
)

set(private_includes)
if (WIN32)
    list(APPEND private_includes 
        "${OCTBIT_THIRD_PARTY_PATH}/GDeflate" 
        "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/include"
    )
endif()

OB_ADD_LIBRARY()

ob_add_test_module()
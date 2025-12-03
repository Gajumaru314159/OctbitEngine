project (Platform)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs Core)
set(private_libs "")

if(UNIX)
    find_package(X11 REQUIRED)
    list(APPEND public_libs X11::X11 glfw)
endif()

OB_ADD_LIBRARY()

ob_add_test_module()
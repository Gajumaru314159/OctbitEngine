project (Input)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs Core Platform)
set(private_libs "")

if(WIN32)
    list(APPEND private_libs dinput8.lib dxguid.lib)
elseif(UNIX)
    find_package(X11 REQUIRED)
    list(APPEND public_libs X11::X11)
endif()

OB_ADD_LIBRARY()
